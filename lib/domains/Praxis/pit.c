//  /domains/Praxis/pit.c
//  A torture chamber, for use with the Nightmare Mortal law system.
//  Created by Manny@Nightmare 940830

#include <lib.h>

inherit LIB_ROOM;

create() {
    ::create();
    SetProperties( ([ "light": 0, "no teleport" : 1, "no magic" : 1
                ]) );
    SetShort("Pit of Spiders");
    SetLong("You are standing deep underground in "
            "a circular pit.  The walls surrounding you are completely "
            "smooth with no sign of hand holds or other ways "
            "to get out.");
    SetItems( ([ ({ "walls", "wall" }) : "A smooth, circular wall, "
                "about 50 cubits deep and unnaturaly smooth." ]) );
}

void reset() {
    int x;

    ::reset();
    if(!present("spider")) {
        for(x=0; x<15; ++x) {
            new("/domains/Praxis/obj/mon/spider")->move(this_object());
        }
    }
}
void init(){
    ::init();
}
