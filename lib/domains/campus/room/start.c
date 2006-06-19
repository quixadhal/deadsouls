#include <lib.h>
#include <message_class.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("LPC University Reception");
    SetLong("You are in the small, spare reception area of the Virtual Campus admissions office. A door leads north to the main administration building corridor.");
    SetNoModify(0);
    SetExits( ([ 
	"north" : "/domains/campus/room/corridor",
      ]) );
    SetDoor("north","/domains/campus/doors/plain_door");
    SetInventory(([
	"/domains/campus/obj/bbucket" :1,
	"/domains/campus/npc/jennybot" :1,
      ]));
    SetProperty("no attack", 1);
}

void init(){
    ::init();
}

mixed CanReceive(object ob){
    if(ob->GetRace() == "verb"){
	return 0;
    }

    if(ob->GetRace() == "rodent"){
	message("info","You are repelled by rodenticide.",ob);
	return 0;
    }
    return 1;
}
