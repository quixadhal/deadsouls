/*    /domains/Ylsrim/room/s_bazaar.c
 *    From the Dead Souls Mud Library
 *    A simple room example with a unique, wandering NPC
 *    Created by Descartes of Borg 960302
 */

#include <lib.h>

inherit LIB_ROOM;

static void create() {
    room::create();
    SetTown("Ylsrim");
    SetClimate("arid");
    SetAmbientLight(30);
    SetShort("The south end of the Ylsrim Bazaar");
    SetLong("Along the south end of the Ylsrim Bazaar stands a restaurant "
      "and a pub.  The central area of the Bazaar is just north of "
      "here.");
    SetSkyDomain("town");
    SetInventory(([
        "/domains/Ylsrim/npc/traveler" : 1,
      ]));
    AddItem("pub", "You can buy yourself an ale from one of Ylsrim's most "
      "respected residents.", ({ "lars" }));
    AddItem("restaurant", "This restaurant is known for its stew.",
      ({ "toral" }));
    SetExits( ([ "north" : "/domains/Ylsrim/room/"+ "bazaar" ]) );
    SetEnters( ([ 
        "pub" : "/domains/Ylsrim/room/"+ "pub" ]) );
}
void init(){
    ::init();
}
