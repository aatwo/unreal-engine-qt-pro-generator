#ifndef DIRECTORYSETUP_H
#define DIRECTORYSETUP_H


#include <QWizardPage>
#include "global.h"


namespace Ui {
    class DirectorySetup;
}
class FilePathEdit;


class DirectorySetup : public QWizardPage
{
    Q_OBJECT
    public:

        explicit            DirectorySetup( Global* global, QWidget* parent = nullptr );
                            ~DirectorySetup();


    signals:

        void                showError( QString error );
        void                showStatus( QString status );


    private:

        virtual void        initializePage() override;
        virtual bool        validatePage() override;

        void                recompute();
        void                onUnrealProjectDirChanged( const QString& dir );

        bool                createQtProjectFile();
        bool                createQtDefinesFile();
        bool                createQtIncludesFile();

        QString             getVisualStudioProjectDefines();
        QString             getVisualStudioProjectIncludes();
        bool                getUserConfirmation( QString title, QString message );

        void                loadResources();
        void                loadSettings();
        void                saveSettings();


        Global*             mGlobal = nullptr;
        Ui::DirectorySetup* ui = nullptr;
        FilePathEdit*       mUnrealProjectDirEdit = nullptr;

        QString             mProFileResource;
        const QString       mProjectNameReplacementTag      = "<projectName>";
        const QString       mVisualStudioDefinesStartTag    = "<NMakePreprocessorDefinitions>";
        const QString       mVisualStudioDefinesEndTag      = "</NMakePreprocessorDefinitions>";
        const QString       mVisualStudioIncludesStartTag   = "<NMakeIncludeSearchPath>";
        const QString       mVisualStudioIncludesEndTag     = "</NMakeIncludeSearchPath>";

        // TODO:    the code to generate the Qt files is done.
        //          next step is to wait for the user to load a tool kit then modify the pro.user
        //          file to match the steps taken in the guide



}; // DirectorySetup


#endif // DIRECTORYSETUP_H
