#ifndef STRINGUTILS_H
#define STRINGUTILS_H


#include <QString>


namespace string_utils
{

    QString             getStringBetweenMarkers( const QString& startTag, const QString& endTag, const QString& input );
    bool                removeTextBetweenMarkers( QString& string_IN, const QString& startMarker, const QString& endMarker, bool removeMarkers );
    bool                replaceTextBetweenMarkers( QString& string_IN, const QString& startMarker, const QString& endMarker, const QString& replacement, bool removeMarkers );


} // string_utils


#endif // STRINGUTILS_H
