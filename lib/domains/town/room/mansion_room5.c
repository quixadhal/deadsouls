#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("The Study");
    SetLong("You are in the study of the master of the house. "+
            "This room is a shambles! It appears to have "+
            "been ransacked in great haste. If there "+
            "ever was anything of value here, it's "+
            "probably long gone.");
    SetExits( ([
                "west" : "/domains/town/room/mansion_uhall3"
                ]) );

    SetDoor("west", "/domains/town/doors/m5.c");

}
void init(){
    ::init();
}
