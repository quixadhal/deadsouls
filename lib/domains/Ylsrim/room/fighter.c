/*    /domains/Ylsrim/room/fighter.c
 *    from the Dead Souls Object Library
 *    created by Descartes of Borg 960512
 */
 
#include <lib.h>

inherit LIB_ROOM;

static void create() {
    room::create();
    SetNoClean(1);
    SetTown("Ylsrim");
    SetClimate("indoors");
    SetAmbientLight(27);
    SetShort("the interior chamber of the Hall of Fighters");
    SetLong("Fighters sort of lounge around in here.");
    SetObviousExits("s");
    SetExits( ([
		"south" : "/domains/Ylsrim/room/"+ "fighter_hall",
    ]) );
    SetItems( ([
	]) );
    SetDoor( "south", "/domains/Ylsrim/etc/fighter_door.c");
}
void init(){
::init();
}
