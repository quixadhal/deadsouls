#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("Corridor");
    SetLong("You are in an east-west corridor on the first floor of the Virtual Campus administrative building. The building's snack bar lies to the north, and a door to the south leads into a stairwell. A foyer and the building exit lie west of here.");
    SetItems( (["corridor" : "This is a carpeted corridor, leading west.",
                "floor" : "The carpet is purple, and somewhat institutional.",
                "carpet" : "The carpet is purple, and somewhat institutional."]) );
    SetExits( ([
                "south" : "/domains/campus/room/stairwell",
                "east" : "/domains/campus/room/corridor",
                "west" : "/domains/campus/room/foyer",
                "north" : "/domains/campus/room/snack.c",
                ]) );
    SetDoor("south","/domains/campus/doors/top_stairs");
    SetProperty("no attack", 1);
}
int CanReceive(object ob){
    if(ob && ob->GetRace() == "rodent"){
        message("info","You are repelled by rodenticide.",ob);
        return 0;
    }
    return ::CanReceive(ob);
}
void init(){
    ::init();
}
