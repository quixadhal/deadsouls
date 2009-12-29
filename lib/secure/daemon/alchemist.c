// File:       /adm/alchemist.c
// Purpose:    Store material properties
//             Setup objects (initialise their properties)
//             This object is also inherited by domain master objects
// Mudlib:     Nightmare
// Author:     Douglas Reay (Pallando @ Ephemeral Dales, Nightmare, etc)
// 93-06-08:   Based on alchemist written by Pallando@FR mud
//             and using the prop_logic seperated by Pallando@TMI-2
// 93-06-16:   Pallando added temperature setting

#include <lib.h>
#include <daemons.h>
#include <materials.h>
inherit LIB_DAEMON;

inherit "/std/prop_logic";

static string *props;      // Names of properties all objects must have
static int num_props;      // number of properties in the above array
mapping properties; // Values of those properties for each material
static string save_file;   // place where the data for the above mapping is kept

mixed get( string property )
{
    mixed parts, value, tmp;
    int size, loop;

    parts = explode( property, "/" );
    size = sizeof( parts );
    if( !mapp( value = properties[parts[0]] ) ) return value;
    for( loop = 1 ; ( loop < size ) && mapp( value ) ; loop++ )
    {
        // Allows default values.  Eg: if we are trying to find the density of lead
        // but that hasn't been set, it will use the default metal density instead
        if( undefinedp( tmp = value[parts[loop]] ) &&
                undefinedp( tmp = value["default"]   )   )
            return mapp( value ) ? tmp : value;
        value = tmp;
    }
    return value;
}

mixed query( string property )
{
    mixed value;

    if( !undefinedp( value = get( property ) ) ) return value;
    return ALCHEMIST_D-> get( property );
}

void init_properties()
{
    string file, property;
    mixed value;
    string *lines;
    int size, loop, tmp;

    properties = ([]);
    if( ( 0    > file_size( save_file+".t" ) ) ||
            !( file = read_file( save_file+".t" ) ) ||
            !( lines = explode( file, "\n" )      ) ||
            !( size = sizeof( lines )             )   ) return;
    properties = allocate_mapping( size );
    for( loop = 0 ; loop < size ; loop++ )
    {
        if( sscanf( lines[loop], "%s;%s", property, value ) < 2 ) continue;
        if( sscanf( value, "%d", tmp ) ) value = tmp;
        // Defined in /std/prop_logic
        // Just does properties[property]=value in a heirachic way (see the .o)
        _set( properties, explode( property, "/" ), value );
    }
    SaveObject( save_file );
}

// During reboots the data is saved in a .o file.
// It only needs to be calculated from the .t when that file is changed
void create()
{
    // Inheriting d_masters use different save files.
    if( !save_file ) save_file = "/data/properties";
    RestoreObject( save_file );
    if( properties ) return;
    init_properties();
}

// This function is called from set_material() in /std/Object
// Inheriting domain master obs can set props in create or in their
// setup_object function, before doing ::setup_object()
void setup_object( object ob, object pobj )
{
    string material, name, type;
    int mass, temperature, loop;

    if( !props )
    {
        props = MATERIAL_PROPS;
        num_props = sizeof( props );
    }
    type = ob-> query( "material_type" );
    if( !type || -1 == member_array( type, MATERIAL_TYPES ) )
    {
        // Log the event here for QC
        return;
    }
    if( !( name = ob-> query( "material_name" ) ) )
    {
        // Log the event here for QC
        return;
    }
    material = "/"+type+"/"+name;
    for( loop = 0 ; loop < num_props ; loop++ )
        ob-> set( props[loop], query( props[loop] + material ) );
    if( !( mass = ob-> query( "mass" ) ) )
    {
        // Log the event here for QC
        return;
    }
    // Until set_weight() is removed.
    ob-> set_weight( mass );
    ob-> set( "volume", 1000 * mass / ( ob-> query( "density" ) ) );
    if( !pobj ) return;
    if( !function_exists( "query", pobj ) ||
            undefinedp( temperature = pobj-> query( "temperature" ) ) )
        temperature = query( "temperature/default" );
    ob-> set( "temperature", temperature );
}
