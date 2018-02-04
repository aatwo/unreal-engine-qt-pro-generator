#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QWizard>
#include "global.h"


class DirectorySetup;
class UnrealInstallationSetup;
class PrepareProUserFile;
class OpenProFilePage;


class MainWindow : public QWizard
{
    Q_OBJECT
    public:

        explicit            MainWindow( QWidget* parent = nullptr );
                            ~MainWindow();


    private:

        void                showStatus( QString status );
        void                showError( QString error );
        void                loadSettings();
        void                saveSettings();
        QWizardPage*        createIntroPage();


        DirectorySetup*             mDirectorySetup             = nullptr;
        UnrealInstallationSetup*    mUnrealInstallationSetup    = nullptr;
        OpenProFilePage*            mOpenProFilePage            = nullptr;
        PrepareProUserFile*         mPrepareProUserFile         = nullptr;
        Global*                     mGlobal                     = nullptr;


}; // MainWindow


#endif // MAINWINDOW_H
