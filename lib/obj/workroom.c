/*    /domains/Examples/room/simple.c
 *    from the Dead Souls LPC Library
 *    a simple example room, bare minimum stuff
 *    created by Descartes of Borg 950402 
 */

#include <lib.h>

inherit LIB_ROOM;

static void create() {
    room::create();
    SetProperties( ([ "light" : 4 ]) );
    SetClimate("indoors");
    SetShort("a workroom");
    SetLong("Endless miles of veldt stretch out underneath an oppressive "
        "sky, awaiting your creative abilities.");
    SetListen("default", "Rustling sounds can be heard in all directions.");
    SetItems( ([ "brush" : "Across the veldt is a high brush which can "
		"conceal about anything.",
		"carnivores" : "You do not see them, but you know they are "
		"here.",
		"sky" : "A vast sky hanging over the endless veldt." ]) );
    SetExits( ([ "out" : "/domains/Praxis/room/square" ]) );
}
