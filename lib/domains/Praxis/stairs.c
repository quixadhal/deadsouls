#include <lib.h>

inherit LIB_ROOM;

void create() {
    ::create();
    SetProperty("light", 1);
    SetProperty("indoors", 1);
    SetProperty("no castle", 1);
    SetShort( "Monastary stairwell");
    SetLong(
            "A spiraling flight of stairs leads up to the monastery "
            "attic and down to the cellar.  The prayer area is west.");
    SetExits( 
            (["west" : "/domains/Praxis/monastery",
             "up" : "/domains/Praxis/monk_join",
             "down" : "/domains/Praxis/immortal_hall"]) );
    SetItems(
            (["stairs" : "They spiral up and down in this old monastery."]) );
    SetProperty("no castle", 1);
}
void init(){
    ::init();
}

