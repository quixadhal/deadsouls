#include <std.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("night light", 1);
    set_property("no castle", 1);
    set_short( "Praxis Commercial District");    
    // set_long(      "Centre Path leads south into the bustling Port of Praxis where it "      "dead ends at Pier 1.  Business life hurries all about you to get "      "things done before the end of the short Praxis day.  In the realty "      ( high mortals make transactions for real estate with "      "the mighty Atmos.  In the great Hall of Fighters northwest fighters "      "gather both night and day.  Praxis centre is northeast from here."    );
    set_long("Centre Path leads south into the bustling Port of Praxis where it "
      "dead ends at Pier 1.  Business life hurries all about you to get "
      "things done before the end of teh short Praxis day.  In the realty "
      "high mortals make transactions for real estate with "
      "the mighty Atmos.  In the great Hall of Fighters northwest fighters "
      "gather both night and day.  Praxis centre is northeast from here.");
    //set_night long",      "Even in the heart of night people are moving about the business "      "district trying to conduct monetary affairs with those types who "      "shy away from daylight.  The shadiest of characters arrive in Praxis "      "at Pier 1 of the Port of Praxis south under cloak of night.  Fighters "      ( especially the nocturnal ones gather in the Hall of "      "Fighters northwest.  Praxis centre is northeast.");

    set_exits( 
      (["northwest" : "/domains/Praxis/fighter_join",
	"northeast" : "/domains/Praxis/s_centre3",
	"east" : "/domains/Praxis/realty",
	"south" : "/domains/Praxis/pier1"]) );
    set_items( 
      ([({"path", "centre path"}) : "It is the main road winding "
	"north and south through Praxis.",
	({"pier", "port", "port of praxis"}) : "Everything that "
	"comes into Praxis from overseas comes through here.",
	({"hall", "hall of fighters"}) : "The fighters of this "
	"reality often come from all over to gather there.",
	"realty" : "High mortals by stuff to build castles with there.", 
	"fighters" : "They are roaming aimlessly about the Hall of "
	"Fighters.",
	"people" : "They are going about their business in the area.",
	"high mortals" : "The mightiest of mortals who often "
	"comprise class leaders."])  );
    set_listen_string("default", "You hear many people going about their "
      "business.");
    set_listen_string("fighters", "They are being really loud and rude.");
    set_smell_string("money", "Damn, you sure are greedy.");
    set_smell_string("fish", "You smell that they have been lying all over "
      "the pier south.");
}

