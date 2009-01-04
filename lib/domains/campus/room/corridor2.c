#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetAmbientLight(30);
    SetClimate("indoors");
    SetShort("Corridor, East");
    SetLong("You are in an east-west corridor on the first floor of the Virtual Campus administrative building. The student lounge is south of here.");
    SetExits( ([ 
                "south" : "/domains/campus/room/lounge",
                "west" : "/domains/campus/room/corridor",
                "east" : "/domains/campus/room/corridor4",
                ]) );

}
void init(){
    ::init();
}
