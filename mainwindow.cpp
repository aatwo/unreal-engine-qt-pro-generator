#include "mainwindow.h"
#include "filepathedit.h"
#include "settings.h"
#include "common.h"
#include "directorysetup.h"
#include "unrealinstallationsetup.h"
#include "openprofilepage.h"
#include "prepareprouserfile.h"
#include <QVBoxLayout>
#include <QLabel>


MainWindow::MainWindow( QWidget* parent )
    : QWizard( parent ),
      mGlobal( new Global() )
{
    setWindowTitle( common::applicationDisplayName );

    QWizardPage* introPage = createIntroPage();
    mDirectorySetup = new DirectorySetup( mGlobal, this );
    mUnrealInstallationSetup = new UnrealInstallationSetup( mGlobal, this );
    mOpenProFilePage = new OpenProFilePage( mGlobal, this );
    mPrepareProUserFile = new PrepareProUserFile( mGlobal, this );

    addPage( introPage );
    addPage( mDirectorySetup );
    addPage( mUnrealInstallationSetup );
    addPage( mOpenProFilePage );
    addPage( mPrepareProUserFile );

    connect( mDirectorySetup, &DirectorySetup::showStatus, this, &MainWindow::showStatus );
    connect( mDirectorySetup, &DirectorySetup::showError, this, &MainWindow::showError );
}

MainWindow::~MainWindow()
{
    saveSettings();
}

void MainWindow::showError( QString error )
{
    // TODO: implement
    //statusBar()->showMessage( error );
}

void MainWindow::showStatus( QString status )
{
    // TODO: implement
    //statusBar()->showMessage( status );
}

void MainWindow::loadSettings()
{
    QByteArray windowGeometry = settings::get( settings::windowGeometry ).toByteArray();
    QByteArray windowState = settings::get( settings::windowState ).toByteArray();

    if( !windowGeometry.isEmpty() )
        restoreGeometry( windowGeometry );
}

void MainWindow::saveSettings()
{
    settings::set( settings::windowGeometry, saveGeometry() );
}

QWizardPage* MainWindow::createIntroPage()
{
    QWizardPage*    page    = new QWizardPage( this );
    QVBoxLayout*    layout  = new QVBoxLayout( page );
    QLabel*         label   = new QLabel( this );
    QString         message = tr( "This wizard will help you setup a Qt Creator project for an Unreal "
                                  "Engine C++ project. This will allow you to launch and debug from "
                                  "within Qt Creator." );

    label->setWordWrap( true );
    label->setText( message );
    layout->addWidget( label );
    return page;
}
