/* 
File created with Roommaker 
written by Haderach@Frontiers 
Room made by Cratylus
Date: Tue Oct 18 02:07:49 2005
WARNING! WARNING! If you edit this room by hand the room maker may be unable to read
it properly. Hand-edit at your own risk.

*/

#include <lib.h>
#include "../customdefs.h"
inherit LIB_ROOM;

static void create() {
    room::create();
    SetTown("fort");
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("fishmonger shop");
    SetLong("This is the fishmonger's shop. The fish here is always fresh, and satisfaction is guaranteed. Fishermen from the nearby eastern seacoast stop here most every day on their way to the lucrative inland markets, and give Henrik sweet deals on their catches. Ice is brought in monthly from the northern island glaciers to keep everything healthy, and Henrik is famous for keeping a surgically clean shop.");
    SetItems(([  "sign" : "This sign lists the fish available for purchase today. Try 'read sign'."]));
    SetInventory(([
	"/domains/fort/npc/henrik" : 1,
      ]));
    SetExits( ([ 
	"north" : "/domains/fort/room/mstreet",
      ]) );

    SetDoor("north", "/domains/fort/doors/fishmonger.c");

}
void init() {
    ::init();
    SetSmell(([]));
    SetListen(([]));
}

int CanReceive(object ob){
    if(ob->GetRace() == "cat"){
	write("Henrik shoos you away!");
	return 0;
    }
    return ::CanReceive();
}
