#include "mainwindow.h"
#include <QApplication>
#include "common.h"


int main( int argc, char* argv[] )
{
    QApplication a( argc, argv );

    a.setApplicationName( common::applicationName );
    a.setApplicationDisplayName( common::applicationDisplayName );
    a.setOrganizationName( common::companyDisplayName );

    MainWindow w;
    w.show();

    return a.exec();
}
