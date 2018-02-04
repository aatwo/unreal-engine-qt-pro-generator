#ifndef PREPAREPROUSERFILE_H
#define PREPAREPROUSERFILE_H


#include <QWizardPage>
#include "global.h"


namespace Ui {
    class PrepareProUserFile;
}


class PrepareProUserFile : public QWizardPage
{
    Q_OBJECT
    public:

                            PrepareProUserFile( Global* global, QWidget* parent = nullptr );


    private:

        virtual void        initializePage() override;
        virtual bool        isComplete() const override;

        void                recompute();
        void                onStartButtonPressed();
        bool                setupBuildSteps( QString& data );
        bool                setupCleanSteps( QString& data );
        bool                setupRunSteps( QString& data );
        void                log( QString message );


        Ui::PrepareProUserFile* ui = nullptr;
        Global*             mGlobal = nullptr;
        bool                mCompleted = false;


}; // PrepareProUserFile


#endif // PREPAREPROUSERFILE_H
