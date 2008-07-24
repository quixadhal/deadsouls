#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("outdoors");
    SetAmbientLight(30);
    SetShort("a generic place");
    SetLong("This is an utterly plain, blank place.");
}
void init(){
    ::init();
}
