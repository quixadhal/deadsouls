// File:       /d/standard/adm/master.c
// Purpose:    The Standard domain's master object.
// Mudlib:     Nightmare
// Author:     Douglas Reay (Pallando @ TMI, Nightmare, etc)
// 93-06-08:   Stores domain specific material properties
//             and does any domain specific initialsation of objects

#include <daemons.h>

inherit ALCHEMIST_D;

void create() { save_file = "/domains/Praxis/data/properties"; ::create(); }

varargs void setup_object( object ob, object pobj )
{
    ::setup_object( ob, pobj );

    // Any domain specific initialisation we wish to do
    ob-> set( "domain", "standard" );
}
