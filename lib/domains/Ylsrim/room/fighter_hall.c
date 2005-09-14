/*    /domains/Ylsrim/room/fighter_hall
 *    from the Dead Souls Object Library
 *    created by Descartes of Borg 960512
 */
 
#include <lib.h>

inherit LIB_ROOM;

int CheckFighter(string dir);

static void create() {
    room::create();
    SetNoClean(1);
    SetTown("Ylsrim");
    SetClimate("indoors");
    SetAmbientLight(27);
    SetShort("the Hall of Fighters");
    SetLong("The Great Hall of Fighters was constructed many ages ago to "
	    "act as a meeting place for all the fighters who come through "
	    "Ylsrim.  In addition, it serves as a place to recruit new "
	    "fighters into the fold.  A magnificent oak door stands north.");
    SetObviousExits("s, n");
    SetExits( ([
		"north" : ({ "/domains/Ylsrim/room/"+ "fighter", (: CheckFighter :) }),
		"south" : "/domains/Ylsrim/room/"+ "kaliid5"
    ]) );
    SetInventory(([ "/domains/Ylsrim/npc/fighter" : 1 ]));
    SetItems( ([
        ({ "meeting","room","hall","fighter hall" }) : 
	"New adventurers may become fighters here.",
	]) );
    SetDoor( "north", "/domains/Ylsrim/etc/fighter_door");
    SetListen("default","Sounds of old veterans recalling battles of "
        "the past can be heard being told.");
}

int CheckFighter(string dir) {
    object ob;

    if( (int)this_player()->ClassMember("fighter") ||
	(int)creatorp(this_player()) ) return 1;
    if( ob = present("fighter", this_object()) ) {
	ob->eventForce("speak I cannot allow you to enter the halls of the great warriors of Ylsrim");
	return 0;
    }
    else return 1;
}

void init(){
::init();
}
