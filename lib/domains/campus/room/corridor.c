#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("Corridor");
    SetLong("You are in an east-west corridor on the first floor of the Virtual Campus administrative building. There is a door south of here, leading into the Admissions office.");
    SetItems( (["corridor" : "This is a carpeted corridor, leading west.",
                "floor" : "The carpet is purple, and somewhat institutional.",
                "carpet" : "The carpet is purple, and somewhat institutional."]) );
    SetExits( ([ 
                "south" : "/domains/campus/room/start",
                "east" : "/domains/campus/room/corridor2",
                "west" : "/domains/campus/room/corridor3",
                ]) );
    SetProperty("no attack", 1);
    SetDoor("south", "/domains/campus/doors/plain_door");
}
int CanReceive(object ob) {
    if(ob && ob->GetRace() == "rodent"){
        message("info","You are repelled by rodenticide.",ob);
        return 0;
    }
    return ::CanReceive(ob);
}
void init(){
    ::init();
}
