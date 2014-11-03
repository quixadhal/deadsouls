#include <lib.h>

inherit LIB_ROOM;

void create() {
    ::create();
    SetProperty("light", 2);
    SetProperty("night light", 1);
    SetProperty("no castle", 1);
    SetShort("A narrow alley");
    SetSmell( "default", "The faint smell of rubbish assaults you.");
    SetLong(
            "You find yourself on a small dirt path which leads to and from "
            "Centre Path.  The path is a dirt road, cluttered occasionally "
            "with garbage from some of the less considerate citizens of Praxis. "
            "The local Post office is to the south, and to the north you can see "
            "a large bulding which serves as the bank of Praxis.");
    SetItems( 
            (["path" : "The path is a small dirt road leading from Centre Path.",
             "bank" :"The bank of Praxis is a very well known establishment.",
             ({"office", "post", "post office"}) : "You can send and receive "
             "mail to other people here or on other muds there.",
             "alley" : "The alley is a very small dirt path."]) );
    SetSkyDomain("town");
    SetExits( ([ 
                "west" : "/domains/Praxis/alley2",
                "east" : "/domains/Praxis/s_centre2",
                ]) );
}
void init(){
    ::init();
}
