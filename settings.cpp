#include "settings.h"
#include <QSettings>
#include "common.h"


void settings::set( const QString& key, QVariant data )
{
    QSettings settings( common::companyName, common::applicationName );
    settings.setValue( key, data );
}

QVariant settings::get( const QString& key, QVariant defaultValue )
{
    QSettings settings( common::companyName, common::applicationName );
    return settings.value( key, defaultValue );
}
