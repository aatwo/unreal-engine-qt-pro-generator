#include "openprofilepage.h"
#include "ui_openprofilepage.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QUrl>
#include <QDesktopServices>


OpenProFilePage::OpenProFilePage( Global* global, QWidget* parent )
    : QWizardPage( parent ),
      mGlobal( global ),
      ui( new Ui::OpenProFilePage )
{
    ui->setupUi( this );

    mDelayTimer = new QTimer( this );
    mDelayTimer->setInterval( 5000 );
    mDelayTimer->setSingleShot( true );
    connect( mDelayTimer, &QTimer::timeout, this, &OpenProFilePage::completeChanged );

    connect( ui->openFolderButton, &QPushButton::clicked, this, &OpenProFilePage::onOpenProjectFolderButtonPressed );
}

void OpenProFilePage::initializePage()
{
    mDelayTimer->start();
    emit completeChanged();
}

bool OpenProFilePage::isComplete() const
{
    return !mDelayTimer->isActive();
}

void OpenProFilePage::onOpenProjectFolderButtonPressed()
{
    QUrl url( mGlobal->projectFileDir );
    QDesktopServices::openUrl( url );
}
