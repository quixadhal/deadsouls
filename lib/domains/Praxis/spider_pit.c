#include <lib.h>
#include <dirs.h>

inherit LIB_ROOM;

create() {
    ::create();
    SetProperties( (["no magic" : 1, "light" : 0, "no teleport" : 1]) );
    SetShort("Pit of Spiders");
    SetLong("You find yourself in the dreaded pit of spiders.  The ground "
            "and walls surrounding you are completely smooth, except for the "
            "crawling lumps of deadly spiders.  By the looks of it, your worst fears "
            "crawling lumps of deadly spiders.  By the looks of it, your "
            "worst fears have come true: there is no escape.");
    SetItems( ([ "spiders" : "Greusome spiders, both dead and alive, "
                "completely cover the ground and walls of the pit.",
                ({ "wall", "walls", "floor" }) : "More spiders." ]) );
}

void reset() {
    int x;
    ::reset();
    if(!present("spider", this_object())) 
        for(x=0; x<10; x++) {
            new("/domains/Praxis/obj/mon/spider")->move(this_object());
        }
}

void init() {
    ::init();
    if(this_player()->query_ghost()) { 
        this_player()->move(DIR_STANDARD_DOMAIN+"/square");
    }
}
