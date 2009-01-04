#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("Grand Ballroom");
    SetLong("This huge room is spectacular both in size "
            "and decor. Clearly the centerpiece of the mansion, "
            "this ballroom could easily accommodate hundreds of "
            "guests. The vaulted ceiling is dozens of feet high, and "
            "the polished marble floors feature beautiful, "
            "dazzling designs.");
    SetExits( ([
                "east" : "/domains/town/room/mansion_dhall3"
                ]) );
}
void init(){
    ::init();
}
