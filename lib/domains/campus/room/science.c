#include <lib.h>
inherit LIB_ROOM;
inherit "/lib/props/ambiance";
static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(20);
    SetShort("a generic room");
    SetLong("This is an utterly plain, blank room.");
}
