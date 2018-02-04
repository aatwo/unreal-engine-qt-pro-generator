#ifndef SETTINGS_H
#define SETTINGS_H


#include <QString>
#include <QVariant>


namespace settings
{

    extern void         set( const QString& key, QVariant data );
    extern QVariant     get( const QString& key, QVariant defaultValue = QVariant() );

    const QString       windowGeometry                  = "windowGeometry";
    const QString       windowState                     = "windowState";
    const QString       lastProjectDirectoryKey         = "lastProjectDirectory";
    const QString       lastUnrealBatDirectoryKey       = "lastUnrealBatDirectory";
    const QString       lastUnrealBinaryDirectoryKey    = "lastUnrealBinaryDirectory";


}


#endif // SETTINGS_H
