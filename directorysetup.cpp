#include "directorysetup.h"
#include "ui_directorysetup.h"
#include "filepathedit.h"
#include "settings.h"
#include "common.h"
#include "stringutils.h"
#include <QStandardPaths>
#include <QDir>
#include <QMessageBox>
#include <QDebug>


DirectorySetup::DirectorySetup( Global* global, QWidget* parent )
    : QWizardPage( parent ),
      ui( new Ui::DirectorySetup ),
      mGlobal( global )
{
    ui->setupUi( this );

    QString filePathEditDialogTitle = tr( "Select your Unreal Engine project root directory..." );
    mUnrealProjectDirEdit = new FilePathEdit( "", filePathEditDialogTitle, this );

    ui->mainLayout->addWidget( mUnrealProjectDirEdit, 2, 1, 1, -1 );
    connect( mUnrealProjectDirEdit, &FilePathEdit::pathChanged, this, &DirectorySetup::onUnrealProjectDirChanged );
    connect( ui->refreshButton, &QPushButton::clicked, this, &DirectorySetup::recompute );

    loadResources();
    loadSettings();
}

DirectorySetup::~DirectorySetup()
{
    saveSettings();
    delete ui;
}

void DirectorySetup::initializePage()
{
    recompute();
}

bool DirectorySetup::validatePage()
{
    { // Check to see if the files exist. If they do then ask for permission to overwrite...
        QFileInfo proFileInfo( mGlobal->qtProjectFile );
        QFileInfo definesFileInfo( mGlobal->qtDefinesFile );
        QFileInfo includesFileInfo( mGlobal->qtIncludesFile );

        if( proFileInfo.exists() || definesFileInfo.exists() || includesFileInfo.exists() )
        {
            QString title = tr( "Overwrite Qt project files?" );
            QString message = tr( "Are you sure you want to overwrite the existing Qt project files?" );

            if( !getUserConfirmation( title, message ) )
                return false;
        }
    }

    if( !createQtProjectFile() || !createQtDefinesFile() || !createQtIncludesFile() )
        return false;

    return true;
}

void DirectorySetup::recompute()
{
    QString dir = mUnrealProjectDirEdit->getFilePath();

    mGlobal->clearProjectPaths();
    mGlobal->unrealProjectDir = dir;

    ui->unrealProjectFile->setText( tr( "unknown" ) );
    ui->projectName->setText( tr( "unknown" ) );
    ui->projectFileDir->setText( tr( "unknown" ) );
    ui->visualStudioFile->setText( tr( "unknown" ) );
    ui->qtProjectFile->setText( tr( "unknown" ) );
    ui->qtDefinesFile->setText( tr( "unknown" ) );
    ui->qtIncludesFile->setText( tr( "unknown" ) );

    bool foundProjectFile = false;
    bool qtProjectFileExists = false;
    bool qtDefinesFileExists = false;
    bool qtIncludesFileExists = false;

    QDir projectDir( mGlobal->unrealProjectDir );
    if( projectDir.exists() )
    {
        // Project name
        auto fileInfoList = projectDir.entryInfoList();
        for( auto fileInfo : fileInfoList )
        {
            if( fileInfo.completeSuffix() != "uproject" )
                continue;

            QString projectName = fileInfo.fileName();
            projectName.remove( QString( ".uproject" ) );
            mGlobal->projectName = projectName;
            foundProjectFile = true;
            break;
        }

        if( foundProjectFile )
        {
            // Project file directory
            {
                QString path = QString( "%1/Intermediate/ProjectFiles" ).arg( mGlobal->unrealProjectDir );
                QFileInfo info( path );
                if( info.exists() )
                    mGlobal->projectFileDir = path;
            }

            // Visual studio file
            {
                QString path = QString( "%1/%2.vcxproj" ).arg( mGlobal->projectFileDir ).arg( mGlobal->projectName );
                QFileInfo info( path );
                if( info.exists() )
                    mGlobal->visualStudioProjectFile = path;
            }

            // Unreal project file
            {
                QString path = QString( "%1/%2.uproject" ).arg( mGlobal->unrealProjectDir ).arg( mGlobal->projectName );
                QFileInfo info( path );
                if( info.exists() )
                    mGlobal->unrealProjectFile = path;
            }

            { // Unreal build dir

                QString path = QString( "%1/Intermediate/Build" ).arg( mGlobal->unrealProjectDir );
                QFileInfo info( path );
                if( info.exists() )
                    mGlobal->unrealBuildDir = path;
            }

            { // Unreal binary dir

                QString path = QString( "%1/Binaries/Win64" ).arg( mGlobal->unrealProjectDir );
                QFileInfo info( path );
                if( info.exists() )
                    mGlobal->unrealBinaryDir = path;
            }

            // Qt project file
            QString qtProjectFilePath = QString( "%1/Intermediate/ProjectFiles/%2.pro" ).arg( mGlobal->unrealProjectDir ).arg( mGlobal->projectName );
            mGlobal->qtProjectFile = qtProjectFilePath;
            QFileInfo qtProjectFileInfo( mGlobal->qtProjectFile );
            qtProjectFileExists = qtProjectFileInfo.exists();

            // Qt defines file
            QString qtDefinesFilePath = QString( "%1/%2UnrealDefines.pri" ).arg( mGlobal->projectFileDir ).arg( mGlobal->projectName );
            mGlobal->qtDefinesFile = qtDefinesFilePath;
            QFileInfo qtDefinesFileInfo( mGlobal->qtDefinesFile );
            qtDefinesFileExists = qtDefinesFileInfo.exists();

            // Qt includes file
            QString qtIncludesFilePath = QString( "%1/%2UnrealIncludes.pri" ).arg( mGlobal->projectFileDir ).arg( mGlobal->projectName );
            mGlobal->qtIncludesFile = qtIncludesFilePath;
            QFileInfo qtIncludesFileInfo( mGlobal->qtIncludesFile );
            qtIncludesFileExists = qtIncludesFileInfo.exists();
        }
    }

    if( !foundProjectFile )
        return;

    if( mGlobal->unrealProjectFile.length() )
        ui->unrealProjectFile->setText( mGlobal->unrealProjectFile );

    if( mGlobal->projectName.length() )
        ui->projectName->setText( mGlobal->projectName );

    if( mGlobal->projectFileDir.length() )
        ui->projectFileDir->setText( mGlobal->projectFileDir );

    if( mGlobal->visualStudioProjectFile.length() )
        ui->visualStudioFile->setText( mGlobal->visualStudioProjectFile );

    if( qtProjectFileExists )
        ui->qtProjectFile->setText( mGlobal->qtProjectFile );
    else
        ui->qtProjectFile->setText( tr( "<will be created>" ) );

    if( qtDefinesFileExists )
        ui->qtDefinesFile->setText( mGlobal->qtDefinesFile );
    else
        ui->qtDefinesFile->setText( tr( "<will be created>" ) );

    if( qtIncludesFileExists )
        ui->qtIncludesFile->setText( mGlobal->qtIncludesFile );
    else
        ui->qtIncludesFile->setText( tr( "<will be created>" ) );

    emit completeChanged();
}

void DirectorySetup::onUnrealProjectDirChanged( const QString& dir )
{
    Q_UNUSED( dir );
    recompute();
}

bool DirectorySetup::createQtProjectFile()
{
    QFile file( mGlobal->qtProjectFile );
    if( !file.open( QFile::WriteOnly ) )
    {
        showError( tr( "Unable to create Qt project file" ) );
        return false;
    }

    QString fileData = mProFileResource;
    fileData.replace( mProjectNameReplacementTag, mGlobal->projectName );
    file.write( fileData.toUtf8() );

    showStatus( tr( "Created Qt project file" ) );
    recompute();

    return true;
}

bool DirectorySetup::createQtDefinesFile()
{
    QFile file( mGlobal->qtDefinesFile );
    if( !file.open( QFile::WriteOnly ) )
    {
        showError( tr( "Unable to create Qt defines file" ) );
        return false;
    }

    QString visualStudioDefines = getVisualStudioProjectDefines();
    QStringList visualStudioDefinesList = visualStudioDefines.split( ";" );
    if( !visualStudioDefinesList.size() )
    {
        showError( tr( "Unable to extract the visual studio defines" ) );
        return false;
    }

    QString fileData = "DEFINES += \\ \n";
    for( int i = 0; i < visualStudioDefinesList.size(); i++ )
    {
        QString define = visualStudioDefinesList.at( i );
        fileData.append( define );

        if( i != visualStudioDefinesList.size() - 1 )
            fileData.append( " \\\n" );
    }

    fileData.append( "\n" );
    file.write( fileData.toUtf8() );

    showStatus( tr( "Created Qt defines file" ) );
    recompute();

    return true;
}

bool DirectorySetup::createQtIncludesFile()
{
    QFile file( mGlobal->qtIncludesFile );
    if( !file.open( QFile::WriteOnly ) )
    {
        showError( tr( "Unable to create Qt includes file" ) );
        return false;
    }

    QString visualStudioIncludes = getVisualStudioProjectIncludes();
    QStringList visualStudioIncludesList = visualStudioIncludes.split( ";" );

    if( !visualStudioIncludesList.size() )
    {
        showError( tr( "Unable to extract the visual studio includes" ) );
        return false;
    }

    QString fileData = "INCLUDEPATH += \\ \n";
    for( int i = 0; i < visualStudioIncludesList.size(); i++ )
    {
        QString include = visualStudioIncludesList.at( i );
        fileData.append( include );

        if( i != visualStudioIncludesList.size() - 1 )
            fileData.append( " \\\n" );
    }

    fileData.append( "\n" );
    file.write( fileData.toUtf8() );

    showStatus( tr( "Created Qt includes file" ) );
    recompute();

    return true;
}

QString DirectorySetup::getVisualStudioProjectDefines()
{
    QFile file( mGlobal->visualStudioProjectFile );
    if( !file.open( QFile::ReadOnly ) )
        return "";

    QString fileData = QString::fromUtf8( file.readAll() );
    return string_utils::getStringBetweenMarkers( mVisualStudioDefinesStartTag, mVisualStudioDefinesEndTag, fileData );
}

QString DirectorySetup::getVisualStudioProjectIncludes()
{
    QFile file( mGlobal->visualStudioProjectFile );
    if( !file.open( QFile::ReadOnly ) )
        return "";

    QString fileData = QString::fromUtf8( file.readAll() );
    return string_utils::getStringBetweenMarkers( mVisualStudioIncludesStartTag, mVisualStudioIncludesEndTag, fileData );
}

bool DirectorySetup::getUserConfirmation( QString title, QString message )
{
    QMessageBox dialog;
    dialog.setWindowTitle( title );
    dialog.setText( message );
    dialog.setStandardButtons( QMessageBox::Ok | QMessageBox::Cancel );
    dialog.setDefaultButton( QMessageBox::Ok );
    int ret = dialog.exec();

    if( ret == QMessageBox::Ok )
        return true;

    return false;
}

void DirectorySetup::loadResources()
{
    QFile proFile( ":/resources/proFile" );
    if( !proFile.open( QFile::ReadOnly ) )
        showError( tr( "Unable to load pro file data" ) );
    else
        mProFileResource = QString::fromUtf8( proFile.readAll() );
}

void DirectorySetup::loadSettings()
{
    QString lastProjectDirectoryPath = settings::get( settings::lastProjectDirectoryKey ).toString();
    if( !lastProjectDirectoryPath.length() )
    {
        QStringList documentLocations = QStandardPaths::standardLocations( QStandardPaths::DocumentsLocation );
        if( documentLocations.size() )
            lastProjectDirectoryPath = documentLocations.first();
    }

    mUnrealProjectDirEdit->setFilePath( lastProjectDirectoryPath );
}

void DirectorySetup::saveSettings()
{
    settings::set( settings::lastProjectDirectoryKey, mUnrealProjectDirEdit->getFilePath() );
}

