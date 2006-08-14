#include <std.h>

inherit ROOM;

void create() {
    object borg;

    ::create();
    set_property("light", 3);
    set_property("night light", 2);
    set_property("no steal", 1);
    set_property("no magic", 1);
    set_property("no attack", 1);
    set_property("no castle", 1);
    set_listen("default", "The sounds of a busy adventuring town are all about.");
    set_short( "Monument Square");
    set_long( "You are in Monument Square, once known as Krasna Square. "
      "The two main roads of Praxis intersect here, where all of "
      "Nightmare's people gather in joy and sorrow.  The road running "
      "north and south is called Centre Path, while Boc La Road is the "
      "name of the road running east and west.");
    set_exits( 
      (["north" : "/domains/Praxis/n_centre1",
	"south" : "/domains/Praxis/s_centre1",
	"east" : "/domains/Praxis/e_boc_la1",
	"west" : "/domains/Praxis/w_boc_la1"]) );

    new("/domains/Praxis/obj/misc/watchtower")->move(this_object());
    set_items(
      (["road" : "Boc La Road leads east toward the adventurer's "
	"hall and the Praxis Monastary.",
	"path" : "Centre Road leads south toward the bank and post office.",
	"square" : "Krasna Square is the central point of Praxis."]) );

    /* The following is to load Manny's dragon. */
    call_other("/realms/manny/guild/room/void", "???");
}

int receive_objects(object ob) { 
    string start;

    if (previous_object()->is_player()) {
	start = previous_object()->getenv("start");
	if (!start) return ::receive_objects(ob);
	if (9>strlen(start)) return ::receive_objects(ob);
	if ("/d/" == start[0..2]) 
	    previous_object()->setenv("start", 
	      "/domains/"+start[3..strlen(start)]);
	if ("d/" == start[0..1]) 
	    previous_object()->setenv("start", 
	      "/domains/"+start[2..strlen(start)]);
	if ("/wizards/" == start[0..8]) 
	    previous_object()->setenv("start", 
	      "/realms/"+start[9..strlen(start)]);
	if ("wizards/" == start[0..7]) 
	    previous_object()->setenv("start", 
	      "/realms/"+start[8..strlen(start)]);
    }
    return ::receive_objects(ob);
}
