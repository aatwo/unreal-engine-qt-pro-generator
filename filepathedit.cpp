#include "filepathedit.h"
#include <QHBoxLayout>
#include <QFileDialog>


FilePathEdit::FilePathEdit( QString initialPath, QString browseDialogTitle, QWidget* parent )
    : QWidget( parent )
{
    mBrowseDialogTitle = browseDialogTitle;
    mPathEdit = new QLineEdit( initialPath, this );
    mBrowseButton = new QPushButton( "...", this );

    auto layout = new QHBoxLayout( this );
    layout->addWidget( mPathEdit );
    layout->addWidget( mBrowseButton );

    setContentsMargins( 0, 0, 0, 0 );
    layout->setContentsMargins( 0, 0, 0, 0 );

    mPathEdit->setPlaceholderText( tr( "Select a folder..." ) );
    mBrowseButton->setSizePolicy( QSizePolicy::Preferred, mBrowseButton->sizePolicy().verticalPolicy() );
    connect( mBrowseButton, &QPushButton::clicked, this, &FilePathEdit::onBrowseButtonPressed );
    connect( mPathEdit, &QLineEdit::textChanged, this, &FilePathEdit::pathChanged );
}

void FilePathEdit::setFilePath( QString path )
{
    mPathEdit->setText( path );
}

QString FilePathEdit::getFilePath()
{
    return mPathEdit->text();
}

void FilePathEdit::onBrowseButtonPressed()
{
    QString newDir = QFileDialog::getExistingDirectory( this, mBrowseDialogTitle, mPathEdit->text() );
    if( !newDir.length() )
        return;

    mPathEdit->setText( newDir );
}
