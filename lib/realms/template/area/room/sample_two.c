#include <lib.h>
#include "../customdefs.h"

inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("Second Sample Room");
    SetLong("This is another sample room.");
    SetItems( ([
                "template" : "That's what this is.",
                ]) );
    SetInventory(([
                MY_OBJ "/case" :1,
                ]));
    SetExits(([
                "west" : MY_AREA "/room/sample_room",
                ]));

}

void init(){
    ::init();
}
