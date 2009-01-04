#include <lib.h>
#include "../customdefs.h"

inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("Sample Room");
    SetLong("This is a room you can use as a template.");
    SetItems( ([
                "template" : "That's what this is.",
                ]) );
    SetExits( ([
                "west" : MY_DIR "/workroom",
                "east" : MY_ROOM "/sample_two.c",
                ]) );
    SetInventory( ([
                MY_OBJ "/table" : 1,
                MY_NPC "/fighter" : 1,
                ]) );
}

void init(){
    ::init();
}
