#include <lib.h>

inherit LIB_ROOM;

void create() {
    ::create();
    SetProperty("light", 1);
    SetProperty("indoors", 1);
    SetProperty("no castle", 1);
    SetShort( "The Hall of Immortals");
    SetLong(
            "Welcome to the hall of immortals!\n"
            "High mortals come here to seek mentors who will guide them into "
            "the world of immortality.  Mortals come here to prove "
            "their worthiness for high mortalhood.  A spiral flight "
            "of stairs leads up out of the cellar.");
    SetExits( 
            (["up" : "/domains/Praxis/stairs"]) );
    SetProperty("no castle", 1);
}

void reset() {
    ::reset();
    if(!present("unity")) 
        new("/domains/Praxis/obj/mon/unity")->move(this_object());
}
void init(){
    ::init();
}
