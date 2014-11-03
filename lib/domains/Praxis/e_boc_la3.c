#include <lib.h>

inherit LIB_ROOM;

void create() {
    ::create();
    SetProperty("light", 3);
    SetProperty("night light", 1);
    SetProperty("no castle", 1);
    SetSmell( "default", "The faint smell of foliage wafts towards you.");
    SetShort( "the east end of Praxis");
    SetLong(
            "The dirt path brings you to the end of the village of Praxis. "
            "Boc La Road meets up with a smaller dirt path which leads "
            "out further east into the wilderness. The path is very "
            "narrow and looks unsafe. After travelling a couple of meters it "
            "is suddenly engulfed in a mass of green leaves and undergrowth. "
            "Up ahead, you can see the darker path of East road, leading to "
            "Praxis' cemetery. West takes you back along East Boc La road.");
    SetItems(
            (["road" : "It leads west toward the village center.",
             "village" : "Praxis is a small adventuring community "
             "of the Nightmare reality.",
             "wilderness" : "Stories tell of bandits and rogues out "
             "in the wilderness.",
             "path" : "The path leads in many directions from here, towards the "
             "Praxis cemetery, the wilderness, and back towards Monument Square.",
             "cemetery": "The Praxis cemetery is off to the north on East Road." ]) );
    SetSkyDomain("town");
    SetExits( ([ 
                "west" : "/domains/Praxis/e_boc_la2",
                "north" : "/domains/Praxis/east_road1",
                "east" : "/domains/Praxis/wild1",
                ]) );
}
void init(){
    ::init();
}
