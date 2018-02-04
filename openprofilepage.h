#ifndef OPENPROFILEPAGE_H
#define OPENPROFILEPAGE_H


#include <QWizardPage>
#include "global.h"
#include <QTimer>


namespace Ui {
    class OpenProFilePage;
}


class OpenProFilePage : public QWizardPage
{
    Q_OBJECT
    public:

        explicit            OpenProFilePage( Global* global, QWidget* parent = nullptr );


    private:

        virtual void        initializePage() override;
        virtual bool        isComplete() const override;
        void                onOpenProjectFolderButtonPressed();

        Global*             mGlobal;
        QTimer*             mDelayTimer;
        Ui::OpenProFilePage* ui;


}; // OpenProFilePage


#endif // OPENPROFILEPAGE_H
