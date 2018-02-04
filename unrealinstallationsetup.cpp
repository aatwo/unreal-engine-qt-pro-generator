#include "unrealinstallationsetup.h"
#include "ui_unrealinstallationsetup.h"
#include <QFileDialog>
#include "settings.h"


UnrealInstallationSetup::UnrealInstallationSetup( Global* global, QWidget* parent ) :
    QWizardPage( parent ),
    ui( new Ui::UnrealInstallationSetup ),
    mGlobal( global )
{
    ui->setupUi( this );

    connect( ui->batDirBrowseButton, &QPushButton::clicked, this, &UnrealInstallationSetup::onBatBrowseButtonPressed );
    connect( ui->batDirEdit, &QLineEdit::textChanged, this, &UnrealInstallationSetup::onBatDirTextChanged );

    connect( ui->binaryDirBrowseButton, &QPushButton::clicked, this, &UnrealInstallationSetup::onBinaryBrowseButtonPressed );
    connect( ui->binaryDirEdit, &QLineEdit::textChanged, this, &UnrealInstallationSetup::onBinaryDirTextChanged );

    loadSettings();
}

UnrealInstallationSetup::~UnrealInstallationSetup()
{
    saveSettings();
    delete ui;
}

void UnrealInstallationSetup::initializePage()
{
    emit completeChanged();
}

bool UnrealInstallationSetup::isComplete() const
{
    QFileInfo batDirInfo( mGlobal->unrealEngineBatDir );
    QFileInfo batBuildInfo( mGlobal->unrealEngineBuildBatFile );
    QFileInfo batCleanInfo( mGlobal->unrealEngineCleanBatFile );

    QFileInfo binaryDirInfo( mGlobal->unrealEngineBinaryDir );
    QFileInfo editorInfo( mGlobal->unrealEngineEditorFile );

    return batDirInfo.exists() && batBuildInfo.exists() && batCleanInfo.exists() && binaryDirInfo.exists() && editorInfo.exists();
}

void UnrealInstallationSetup::loadSettings()
{
    QString batDir = settings::get( settings::lastUnrealBatDirectoryKey, "" ).toString();
    QString binDir = settings::get( settings::lastUnrealBinaryDirectoryKey, "" ).toString();

    if( !batDir.length() )
    {
        // Try normal dir
        QString dir = "C:\\Program Files\\Epic Games\\UE_4.17\\Engine\\Build\\BatchFiles";
        QFileInfo info( dir );
        if( info.exists() )
            batDir = dir;
    }

    if( !batDir.length() )
    {
        // Try unreal root install dir
        QString dir = "C:\\Program Files\\Epic Games";
        QFileInfo info( dir );
        if( info.exists() )
            batDir = dir;
    }

    if( !binDir.length() )
    {
        // Try normal dir
        QString dir = "C:\\Program Files\\Epic Games\\UE_4.17\\Engine\\Binaries\\Win64";
        QFileInfo info( dir );
        if( info.exists() )
            binDir = dir;
    }

    if( !binDir.length() )
    {
        // Try unreal root install dir
        QString dir = "C:\\Program Files\\Epic Games";
        QFileInfo info( dir );
        if( info.exists() )
            binDir = dir;
    }

    ui->batDirEdit->setText( batDir );
    ui->binaryDirEdit->setText( binDir );
}

void UnrealInstallationSetup::saveSettings()
{
    settings::set( settings::lastUnrealBatDirectoryKey, mGlobal->unrealEngineBatDir );
    settings::set( settings::lastUnrealBinaryDirectoryKey, mGlobal->unrealEngineBinaryDir );
}

void UnrealInstallationSetup::onBatBrowseButtonPressed()
{
    QString caption = tr( "Select your Unreal installations BAT file directory..." );
    QString dir = QFileDialog::getExistingDirectory( this, caption, mGlobal->unrealEngineBatDir );
    if( dir.length() )
        ui->batDirEdit->setText( dir );
}

void UnrealInstallationSetup::onBatDirTextChanged( const QString& dir )
{
    mGlobal->unrealEngineBatDir = dir;
    mGlobal->unrealEngineBuildBatFile = QString( "%1\\Build.bat" ).arg( dir );
    mGlobal->unrealEngineCleanBatFile = QString( "%1\\Clean.bat" ).arg( dir );
    emit completeChanged();
}

void UnrealInstallationSetup::onBinaryBrowseButtonPressed()
{
    QString caption = tr( "Select your Unreal installations binary directory..." );
    QString dir = QFileDialog::getExistingDirectory( this, caption, mGlobal->unrealEngineBinaryDir );
    if( dir.length() )
        ui->binaryDirEdit->setText( dir );
}

void UnrealInstallationSetup::onBinaryDirTextChanged( const QString& dir )
{
    mGlobal->unrealEngineBinaryDir = dir;
    mGlobal->unrealEngineEditorFile = QString( "%1\\UE4Editor.exe" ).arg( dir );
    emit completeChanged();
}
