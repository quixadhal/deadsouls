/*    /secure/sefun/identify.c
 *    from the Dead Souls LPC Library
 *    represent a variable of any type as a string, now out of date
 *    remains for backwards compat
 *    created by Pallando@Dead Souls (Douglas Reay) 921212
 */

varargs string identify( mixed a )
{
    int i, s;
    string ret;
    mapping RealMap;

    if( undefinedp( a ) ) return "UNDEFINED";
    if( nullp( a ) ) return "0";
    if( intp( a ) ) return "" + a;
    if( floatp( a ) ) return "" + a;
    if( objectp( a ) )
    {
        if( ret = a-> GetKeyName() ) ret += " ";
        else ret = "";
        return "OBJ(" + ret + file_name( a ) + ")";
    }
    if( stringp( a ) )
    {
        a = replace_string( a, "\"", "\\\"" );
        a = "\"" + a + "\"";
        a = replace_string( a, "\\", "\\\\" );
        a = replace_string( a, "\\\"", "\"" );
        a = replace_string( a, "\n", "\\n" );
        a = replace_string( a, "\t", "\\t" );
        return a;
    }
    if(classp(a)){
        ret = replace_string(sprintf("%O",a),"\n","");
        return ret;
    }
    if( pointerp( a ) ) 
    {
        ret = "({ ";
        s = sizeof( a );
        for( i = 0 ; i < s ; i++ )
        {
            if( i ) ret += ", ";
            ret += identify( a[i] );
        }
        return ret + ( s ? " " : "" ) + "})";
    }
    if( mapp( a ) )
    {
        ret = "([ ";
        RealMap = (a);
        a = keys( RealMap );
        s = sizeof( a );
        for( i = 0 ; i < s ; i++ )
        {
            if( i ) ret += ", ";
            ret += identify( a[i] ) + " : " + identify( RealMap[a[i]] );
        }
        return ret + ( s ? " " : "" ) + "])";
    }
    if(functionp(a)) return sprintf("%O", a);
    return "UNKNOWN";
}
