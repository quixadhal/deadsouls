#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetTown("town");
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("Village Schoolhouse");
    SetLong("This simple, one-room schoolhouse is not especially large but it is clean, well-maintained, and clearly an important part of village life. People come here to be taught, mostly languages, by traveling teachers and guest lecturers.");
    SetProperties (([
                "no attack":1, 
                "no bump":1, 
                "no steal":0, 
                "no magic":0, 
                "no paralyze":0, 
                "no teleport":0]));
    SetItems(([]));
    SetInventory(([
                "/domains/town/npc/bugg" : ({60, 1}),
                ]));
    SetExits(([
                "south" : "/domains/town/room/vill_road4",
                ]));

}
void init() {
    ::init();
    SetSmell(([]));
    SetListen(([]));
}
