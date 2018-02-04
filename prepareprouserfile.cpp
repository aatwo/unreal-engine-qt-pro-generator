#include "prepareprouserfile.h"
#include "ui_prepareprouserfile.h"
#include <QFileInfo>
#include "stringutils.h"


PrepareProUserFile::PrepareProUserFile( Global* global, QWidget* parent )
    : QWizardPage( parent ),
      ui( new Ui::PrepareProUserFile ),
      mGlobal( global )
{
    ui->setupUi( this );

    connect( ui->startButton, &QPushButton::clicked, this, &PrepareProUserFile::onStartButtonPressed );
}

void PrepareProUserFile::initializePage()
{
    recompute();
    mCompleted = false;
}

bool PrepareProUserFile::isComplete() const
{
    return mCompleted;
}

void PrepareProUserFile::recompute()
{
    emit completeChanged();
}

void PrepareProUserFile::onStartButtonPressed()
{
    mCompleted = false;
    emit completeChanged();
    log( tr( "Processing pro.user file..." ) );

    QString proUserFilePath = QString( "%1/%2.pro.user" ).arg( mGlobal->projectFileDir ).arg( mGlobal->projectName );
    QFileInfo fileInfo( proUserFilePath );
    QFile file( proUserFilePath );

    if( !fileInfo.exists() )
    {
        log( tr( "Error: File not found." ) );
        return;
    }

    if( !file.open( QFile::ReadOnly ) )
    {
        log( tr( "Error: unable to open file." ) );
        return;
    }

    QByteArray  data        = file.readAll();
    QString     dataString  = QString::fromUtf8( data );

    file.close();

    // Process the file...
    if( !setupBuildSteps( dataString ) || !setupCleanSteps( dataString ) || !setupRunSteps( dataString ) )
    {
        log( "Error: unable to make changes." );
        return;
    }

    if( !file.open( QFile::WriteOnly ) )
    {
        log( tr( "Error: unable to write changes." ) );
        return;
    }

    if( file.write( dataString.toUtf8() ) != dataString.size() )
    {
        log( tr( "Error: and error occured when writing the changes." ) );
        return;
    }

    log( tr( "Finished writing changes." ) );
    mCompleted = true;
    emit completeChanged();
}

bool PrepareProUserFile::setupBuildSteps( QString& data )
{
    QString projectName             = mGlobal->projectName;
    QString unrealProjectFilePath   = mGlobal->unrealProjectFile;
    QString buildDir                = mGlobal->unrealBuildDir;
    QString buildBat                = mGlobal->unrealEngineBuildBatFile;

    QString buildStepListStart  = "<valuemap type=\"QVariantMap\" key=\"ProjectExplorer.BuildConfiguration.BuildStepList.0\">";
    QString buildStepListEnd    = "<value type=\"int\" key=\"ProjectExplorer.BuildStepList.StepsCount\">";
    QString newBuildStepString  = QString(  "<valuemap type=\"QVariantMap\" key=\"ProjectExplorer.BuildStepList.Step.0\">"
                                                "<value type=\"bool\" key=\"ProjectExplorer.BuildStep.Enabled\">true</value>"
                                                "<value type=\"QString\" key=\"ProjectExplorer.ProcessStep.Arguments\">%1 Win64 DebugGame &quot;%2&quot; -rocket</value>"
                                                "<value type=\"QString\" key=\"ProjectExplorer.ProcessStep.Command\">%3</value>"
                                                "<value type=\"QString\" key=\"ProjectExplorer.ProcessStep.WorkingDirectory\">%4</value>"
                                                "<value type=\"QString\" key=\"ProjectExplorer.ProjectConfiguration.DefaultDisplayName\">Custom Process Step</value>"
                                                "<value type=\"QString\" key=\"ProjectExplorer.ProjectConfiguration.DisplayName\"></value>"
                                                "<value type=\"QString\" key=\"ProjectExplorer.ProjectConfiguration.Id\">ProjectExplorer.ProcessStep</value>"
                                            "</valuemap>" ).arg( projectName ).arg( unrealProjectFilePath ).arg( buildBat ).arg( buildDir );

    bool replaceResult = string_utils::replaceTextBetweenMarkers( data, buildStepListStart, buildStepListEnd, newBuildStepString, false );
    if( !replaceResult )
    {
        log( "Error: unable to replace the build step list." );
        return false;
    }

    { // Update the existing build step count

        QString start = "<value type=\"int\" key=\"ProjectExplorer.BuildStepList.StepsCount\">";
        QString end = "</value>";

        bool replacedOk = string_utils::replaceTextBetweenMarkers( data, start, end, "1", false );
        if( !replacedOk )
        {
            log( tr( "Error: unable to update the build step count." ) );
            return false;
        }
    }

    log( tr( "Finished writing new build steps..." ) );
    return true;
}

bool PrepareProUserFile::setupCleanSteps( QString& data )
{
    QString cleanBat            = mGlobal->unrealEngineCleanBatFile;
    QString cleanStepListStart  = "<valuemap type=\"QVariantMap\" key=\"ProjectExplorer.BuildConfiguration.BuildStepList.1\">";
    QString cleanStepListEnd    = "<value type=\"int\" key=\"ProjectExplorer.BuildStepList.StepsCount\">";
    QString newCleanStepString  = QString(  "<valuemap type=\"QVariantMap\" key=\"ProjectExplorer.BuildStepList.Step.0\">"
                                                "<value type=\"bool\" key=\"ProjectExplorer.BuildStep.Enabled\">true</value>"
                                                "<value type=\"QString\" key=\"ProjectExplorer.ProcessStep.Arguments\"></value>"
                                                "<value type=\"QString\" key=\"ProjectExplorer.ProcessStep.Command\">%1</value>"
                                                "<value type=\"QString\" key=\"ProjectExplorer.ProcessStep.WorkingDirectory\">%{buildDir}</value>"
                                                "<value type=\"QString\" key=\"ProjectExplorer.ProjectConfiguration.DefaultDisplayName\">Custom Process Step</value>"
                                                "<value type=\"QString\" key=\"ProjectExplorer.ProjectConfiguration.DisplayName\"></value>"
                                                "<value type=\"QString\" key=\"ProjectExplorer.ProjectConfiguration.Id\">ProjectExplorer.ProcessStep</value>"
                                            "</valuemap>" ).arg( cleanBat );

    bool replaceResult = string_utils::replaceTextBetweenMarkers( data, cleanStepListStart, cleanStepListEnd, newCleanStepString, false );
    if( !replaceResult )
    {
        log( tr( "Error: unable to replace the clean step list." ) );
        return false;
    }

    log( tr( "Finished writing new clean steps..." ) );
    return true;
}

bool PrepareProUserFile::setupRunSteps( QString& data )
{
    QString runConfigCountXml = "<value type=\"int\" key=\"ProjectExplorer.Target.RunConfigurationCount\">1</value>";

    { // Check the xml is present

        int index = data.indexOf( runConfigCountXml );
        if( index == -1 )
        {
            log( tr( "Error: unable to locate the existing run configuration." ) );
            return false;
        }
    }

    QString runStepConfigReplacement = QString(

        "<valuemap type=\"QVariantMap\" key=\"ProjectExplorer.Target.RunConfiguration.1\">"
            "<value type=\"bool\" key=\"Analyzer.QmlProfiler.AggregateTraces\">false</value>"
            "<value type=\"bool\" key=\"Analyzer.QmlProfiler.FlushEnabled\">false</value>"
            "<value type=\"uint\" key=\"Analyzer.QmlProfiler.FlushInterval\">1000</value>"
            "<value type=\"QString\" key=\"Analyzer.QmlProfiler.LastTraceFile\"></value>"
            "<value type=\"bool\" key=\"Analyzer.QmlProfiler.Settings.UseGlobalSettings\">true</value>"
            "<valuelist type=\"QVariantList\" key=\"Analyzer.Valgrind.AddedSuppressionFiles\"/>"
            "<value type=\"bool\" key=\"Analyzer.Valgrind.Callgrind.CollectBusEvents\">false</value>"
            "<value type=\"bool\" key=\"Analyzer.Valgrind.Callgrind.CollectSystime\">false</value>"
            "<value type=\"bool\" key=\"Analyzer.Valgrind.Callgrind.EnableBranchSim\">false</value>"
            "<value type=\"bool\" key=\"Analyzer.Valgrind.Callgrind.EnableCacheSim\">false</value>"
            "<value type=\"bool\" key=\"Analyzer.Valgrind.Callgrind.EnableEventToolTips\">true</value>"
            "<value type=\"double\" key=\"Analyzer.Valgrind.Callgrind.MinimumCostRatio\">0.01</value>"
            "<value type=\"double\" key=\"Analyzer.Valgrind.Callgrind.VisualisationMinimumCostRatio\">10</value>"
            "<value type=\"bool\" key=\"Analyzer.Valgrind.FilterExternalIssues\">true</value>"
            "<value type=\"int\" key=\"Analyzer.Valgrind.LeakCheckOnFinish\">1</value>"
            "<value type=\"int\" key=\"Analyzer.Valgrind.NumCallers\">25</value>"
            "<valuelist type=\"QVariantList\" key=\"Analyzer.Valgrind.RemovedSuppressionFiles\"/>"
            "<value type=\"int\" key=\"Analyzer.Valgrind.SelfModifyingCodeDetection\">1</value>"
            "<value type=\"bool\" key=\"Analyzer.Valgrind.Settings.UseGlobalSettings\">true</value>"
            "<value type=\"bool\" key=\"Analyzer.Valgrind.ShowReachable\">false</value>"
            "<value type=\"bool\" key=\"Analyzer.Valgrind.TrackOrigins\">true</value>"
            "<value type=\"QString\" key=\"Analyzer.Valgrind.ValgrindExecutable\">valgrind</value>"
            "<valuelist type=\"QVariantList\" key=\"Analyzer.Valgrind.VisibleErrorKinds\">"
             "<value type=\"int\">0</value>"
             "<value type=\"int\">1</value>"
             "<value type=\"int\">2</value>"
             "<value type=\"int\">3</value>"
             "<value type=\"int\">4</value>"
             "<value type=\"int\">5</value>"
             "<value type=\"int\">6</value>"
             "<value type=\"int\">7</value>"
             "<value type=\"int\">8</value>"
             "<value type=\"int\">9</value>"
             "<value type=\"int\">10</value>"
             "<value type=\"int\">11</value>"
             "<value type=\"int\">12</value>"
             "<value type=\"int\">13</value>"
             "<value type=\"int\">14</value>"
            "</valuelist>"
            "<value type=\"int\" key=\"PE.EnvironmentAspect.Base\">2</value>"
            "<valuelist type=\"QVariantList\" key=\"PE.EnvironmentAspect.Changes\"/>"
            "<value type=\"QString\" key=\"ProjectExplorer.CustomExecutableRunConfiguration.Arguments\">&quot;%1&quot; -debug</value>"
            "<value type=\"QString\" key=\"ProjectExplorer.CustomExecutableRunConfiguration.Executable\">%2</value>"
            "<value type=\"QString\" key=\"ProjectExplorer.CustomExecutableRunConfiguration.WorkingDirectory\">%3</value>"
            "<value type=\"QString\" key=\"ProjectExplorer.ProjectConfiguration.DefaultDisplayName\">Run %2</value>"
            "<value type=\"QString\" key=\"ProjectExplorer.ProjectConfiguration.DisplayName\">Debug Editor</value>"
            "<value type=\"QString\" key=\"ProjectExplorer.ProjectConfiguration.Id\">ProjectExplorer.CustomExecutableRunConfiguration</value>"
            "<value type=\"uint\" key=\"RunConfiguration.QmlDebugServerPort\">3768</value>"
            "<value type=\"bool\" key=\"RunConfiguration.UseCppDebugger\">false</value>"
            "<value type=\"bool\" key=\"RunConfiguration.UseCppDebuggerAuto\">true</value>"
            "<value type=\"bool\" key=\"RunConfiguration.UseMultiProcess\">false</value>"
            "<value type=\"bool\" key=\"RunConfiguration.UseQmlDebugger\">false</value>"
            "<value type=\"bool\" key=\"RunConfiguration.UseQmlDebuggerAuto\">true</value>"
           "</valuemap>"
            "<value type=\"int\" key=\"ProjectExplorer.Target.RunConfigurationCount\">2</value>"

        ).arg( mGlobal->unrealProjectFile ).arg( mGlobal->unrealEngineEditorFile ).arg( mGlobal->unrealBinaryDir );

    data.replace( runConfigCountXml, runStepConfigReplacement );

    { // Replace the active run configuration

       QString start = "<value type=\"int\" key=\"ProjectExplorer.Target.ActiveRunConfiguration\">";
       QString end = "</value>";

       bool replacedOk = string_utils::replaceTextBetweenMarkers( data, start, end, "1", false );
       if( !replacedOk )
       {
           log( tr( "Error: unable to set the new rune configuration." ) );
           return false;
       }
    }

    log( tr( "Finished replacing the run configuration..." ) );
    return true;
}

void PrepareProUserFile::log( QString message )
{
    ui->log->appendPlainText( message );
}
