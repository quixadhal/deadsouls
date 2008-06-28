#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("indoors");
    SetProperties( ([ "light" : 2 ]) );
    SetShort("a generic room");
    SetLong("This is an utterly plain, blank room.");
}
void init(){
    ::init();
}
