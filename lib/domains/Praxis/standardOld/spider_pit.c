#include <std.h>
#include <dirs.h>

inherit ROOM;

create() {
    ::create();
    set_properties( (["no magic" : 1, "light" : 0, "no teleport" : 1]) );
    set_short("Pit of Spiders");
    set_long("You find yourself in the dreaded pit of spiders.  The ground "
      "and walls surrounding you are completely smooth, except for the "
      "crawling lumps of deadly spiders.  By the looks of it, your worst fears "
      "crawling lumps of deadly spiders.  By the looks of it, your "
      "worst fears have come true: there is no escape.");
    set_items( ([ "spiders" : "Greusome spiders, both dead and alive, "
	"completely cover the ground and walls of the pit.",
	({ "wall", "walls", "floor" }) : "More spiders." ]) );
}

reset() {
    int x;
    ::reset();
    if(!present("spider", this_object())) 
	for(x=0; x<10; x++) {
	    new("/"+__DIR__+"obj/mon/spider")->move(this_object());
	}
}

init() {
    ::init();
    if(this_player()->query_ghost()) { 
	this_player()->move(DIR_STANDARD_DOMAIN+"/square");
    }
}
