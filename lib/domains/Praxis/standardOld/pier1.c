#include <std.h>

inherit PIER;

void create() {
    ::create();
    set_property("no castle", 1);
    set_property("light", 3);
    set_property("night light", 2);
    set_max_catch(15);
    set_max_fishing(4);
    set_chance(10);
    set_short( "Pier 1 of the Port of Praxis");
    set_long(
      "Welcome to the Port of Praxis!\n"
      "Ships from all over reality dock here to deliver goods to "
      "Praxis and the surrounding region.  All along the pier "
      "are huge torches which light up the pier like day during "
      "the night time hours so that people may unload goods 20 hours "
      "a day.  People are also fishing everywhere along the pier. "
    );
    set_smell("default", "Rotting fish offend your nostrils.");
    set_smell("fish", "They smell as if they have been out for days.");
    set_items(
      (["port" : "Ships from all over Nightmare dock here to "
	"do business in Praxis.",
	"pier" : "Pier 1 of the port.",
	"goods" : "Merchandise from exotic places.",
	"torch" : "It lights up the pier at night.",
	"torches" : "They light up the pier at night."]) );
    set_exits( 
      (["north" : "/domains/Praxis/s_centre4" 	
      ]) );
}

void reset() {
    int i;

    ::reset();
    if(!present("stick"))
	new("/domains/Praxis/obj/misc/fishing_pole")->move(this_object());
    if(present("match")) return;
    i = 4;
    while(i--) new("/domains/Praxis/obj/misc/match")->move(this_object());
}

