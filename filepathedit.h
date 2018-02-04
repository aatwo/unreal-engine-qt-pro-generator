#ifndef FILEPATHEDIT_H
#define FILEPATHEDIT_H


#include <QWidget>
#include <QLineEdit>
#include <QPushButton>


class FilePathEdit : public QWidget
{
    Q_OBJECT
    public:

        explicit            FilePathEdit( QString initialPath, QString browseDialogTitle, QWidget* parent = nullptr );

        void                setFilePath( QString path );
        QString             getFilePath();


    signals:

        void                pathChanged( const QString& path );


    private:

        void                onBrowseButtonPressed();


        QLineEdit*          mPathEdit;
        QPushButton*        mBrowseButton;
        QString             mBrowseDialogTitle;


}; // FilePathEdit

#endif // FILEPATHEDIT_H
