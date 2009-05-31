#include <lib.h>
inherit LIB_ROOM;
inherit "/lib/props/ambiance";
static void create() {
    room::create();
    SetClimate("outdoors");
    SetAmbientLight(25);
    SetShort("Fitzpatrick Hall");
    SetLong("This is the messy foyer of a building under construction.");
    SetExits( ([ "north" : "/domains/campus/room/usquare",
                ]) );
}
int CanReceive(object ob) {
    message("info","The classroom building is temporarily "
            "closed for renovation.", ob);
    return 0;
}
