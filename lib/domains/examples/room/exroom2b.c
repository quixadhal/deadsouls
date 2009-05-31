#include <lib.h>
#include "ex.h"

inherit PAPAROOM;

void create()
{
    ::create();
    SetAmbientLight(30);
    SetShort( "a 'child' file" );
    SetLong("This is an example room that inherits the properties "
            "of another room.");

    /*  Note:  When we set the "exits" in this room we over-write
        the "exits" we inherited from PAPAROOM.  There's a way to
        just add an exit to the list, but exits are primarily
        unique to each room, so this method is fine.
     */

    SetExits( ([
                "west" : EXPATH + "exroom2a",
                ]));

    /*  Now, we are setting the objects that are loaded into the room,
        or the inventory of the room.  Just like a player has inventory
        on their character.
     */

    SetInventory(  ([ "/domains/default/obj/bbucket" : 1 ]));

}


void init()
{
    ::init();
}
