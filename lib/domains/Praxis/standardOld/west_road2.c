#include <std.h>

inherit ROOM;

int dug;
string orig_long, dug_long;

void init() {
    ::init();
    add_action("read", "read");
    add_action("dig", "dig");
}

void create() {
    ::create();
    set_property("light", 2);
    set_property("no castle", 1);
    set_short( "Along West Road");
    set_long( (: this_object(), "is_dug" :));
    set_no_clean(1);
    set_items(
      (["road" : "There is a mound of loose dirt piled by its side.",
	"dirt" : "It is very loose.",
	"hut " : "It looks very mystical.",
	"sign" : "Those who can read are very fortunate indeed!",
	"mound" : (: this_object(), "look_at_mound" :) ]) );
    set_exits( 
      (["north" : "/domains/Praxis/west_road3",
	"south" : "/domains/Praxis/west_road1",
	"east" : "/domains/Praxis/medium"]) );
    dug = 0;
}

void reset() {
    ::reset();
    if(!dug) return;
    dug = 0;
    remove_item_description("hole");
    remove_exit("down");
    call_other("/domains/Praxis/hole", "fixing_a_hole");
}

void look_at_mound() {
    if(!dug) write("It looks as if someone dug a hole there, "
	  "then covered it back up.");
    else write("It is a mound of dirt dug out from the hole beside it.");
}

int dig() {
    if(!present("shovel", this_player()) && !present("spade", this_player())) {
	notify_fail("You have nothing to dig with!\n");
	return 0;
    }
    if(dug) {
	notify_fail("Where are you going to dig?\n");
	return 0;
    }
    write("You dig into the mound and uncover a hole.");
    say(this_player()->query_cap_name()+" digs into the mound "
      "and uncovers a hole.", this_player());
    dug = 1;
    add_item_description("hole", "A hole where someone just dug into the ground by the mound by the road.");    add_exit( "down", "/domains/Praxis/hole");

    call_other("/domains/Praxis/hole", "digging");
    return 1;
}

int query_dug() { return dug; }

int read(string str) {
    if(str != "sign" && str != "door") {
	notify_fail("Read what?\n");
	return 0;
    }
    write("Mora's house of mysticism.\n"
      "Learn yours or someone else's horoscope for the day.");
    return 1;
}

string is_dug() {
    if(!dug) return "You are in the middle of West Road where rural "
	"huts line the sides of the road.  One of the huts on the east side "
	"has a sign hanging outside it.  A mound of loose dirt covers part "
	"of the west side of the road.";
    else return "You are in the middle of west road where rural huts line "
	"the sides of the road.  One of the huts on the east side has a "
	"sign hanging outside of it.  A hole has been recently uncovered "
	"on the west side of the road.";
}

