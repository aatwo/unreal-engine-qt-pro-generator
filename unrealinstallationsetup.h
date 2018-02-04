#ifndef UNREALINSTALLATIONSETUP_H
#define UNREALINSTALLATIONSETUP_H


#include <QWizardPage>
#include "global.h"


namespace Ui {
    class UnrealInstallationSetup;
}


class UnrealInstallationSetup : public QWizardPage
{
    Q_OBJECT
    public:

        explicit            UnrealInstallationSetup( Global* global, QWidget* parent = nullptr );
                            ~UnrealInstallationSetup();


    private:

        virtual void        initializePage() override;
        virtual bool        isComplete() const override;

        void                loadSettings();
        void                saveSettings();

        void                onBatBrowseButtonPressed();
        void                onBatDirTextChanged( const QString& dir );

        void                onBinaryBrowseButtonPressed();
        void                onBinaryDirTextChanged( const QString& dir );


        Ui::UnrealInstallationSetup* ui = nullptr;
        Global*             mGlobal = nullptr;


}; // UnrealInstallationSetup


#endif // UNREALINSTALLATIONSETUP_H
