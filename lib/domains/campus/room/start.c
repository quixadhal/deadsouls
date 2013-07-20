#include <lib.h>
#include <message_class.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("LPC University Reception");
    SetLong("You are in the small, spare reception area of the Virtual Campus admissions office. A door leads north to the main administration building corridor.");
    SetExits( ([
                "north" : "/domains/campus/room/corridor",
                ]) );
    SetNoModify(1);
    SetDoor("north","/domains/campus/doors/plain_door");
    SetInventory(([
                "/domains/campus/obj/bbucket" :1,
                "/domains/campus/npc/jennybot" : ({60, 1}),
                ]));
    SetProperty("no attack", 1);
    SetCoordinates("4000,4000,0");
}

void init(){
    ::init();
}

mixed CanReceive(object ob){
    if(ob && ob->GetRace() == "rodent"){
        message("info","You are repelled by rodenticide.",ob);
        return 0;
    }
    return ::CanReceive(ob);
}
