#include "stringutils.h"


QString string_utils::getStringBetweenMarkers( const QString& startTag, const QString& endTag, const QString& input )
{
    int startIndex = input.indexOf( startTag );
    if( startIndex == -1 )
        return "";

    startIndex += startTag.length();

    int endIndex = input.indexOf( endTag, startIndex );
    if( endIndex == -1 )
        return "";

    int subStringLength = endIndex - startIndex;

    if( subStringLength <= 0 )
        return "";

    return input.mid( startIndex, endIndex - startIndex );
}

bool string_utils::removeTextBetweenMarkers( QString& string_IN, const QString& startMarker, const QString& endMarker, bool removeMarkers ) {

    int startIndex = string_IN.indexOf( startMarker );
    if( startIndex == -1 )
        return false;

    int endIndex = string_IN.indexOf( endMarker, startIndex );
    if( endIndex == -1 )
        return false;

    if( !removeMarkers )
        startIndex += startMarker.length();

    int cutLength = endIndex - startIndex;

    if( removeMarkers )
        cutLength += endMarker.length();

    string_IN.remove( startIndex, cutLength );

    return true;
}

bool string_utils::replaceTextBetweenMarkers( QString& string_IN, const QString& startMarker, const QString& endMarker, const QString& replacement, bool removeMarkers ) {

    int startIndex = string_IN.indexOf( startMarker );
    if( startIndex == -1 )
        return false;

    int endIndex = string_IN.indexOf( endMarker, startIndex );
    if( endIndex == -1 )
        return false;

    if( !removeMarkers )
        startIndex += startMarker.length();

    int cutLength = endIndex - startIndex;

    if( removeMarkers )
        cutLength += endMarker.length();

    string_IN.remove( startIndex, cutLength );
    string_IN.insert( startIndex, replacement );

    return true;
}
