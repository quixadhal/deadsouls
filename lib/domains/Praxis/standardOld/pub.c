#include <std.h>

inherit ROOM;

void init() {
    ::init();
    add_action("read", "read");
}

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 1);
    set_short( "Lars Pub");
    set_long(
      "Welcome to the Lars Pub!\n"
      "The Lars Pub was established many many years ago, "
      "in the time before reality.  These days it is showing "
      "its age, and is a bit outdated, but it is still a fun "
      "place to grab a drink.  A list of drinks is on the wall.");
    set_items(
      (["pub" : "The most hoppin' place in Praxis.",
	"bar" : "The Lars Pub.",
	"foo" : "bar",
	"list" : "It looks like a list of drinks. Read it to see "
	"what's available."]) );
    set_exits( 
      (["north" : "/domains/Praxis/w_boc_la1",
	"east" : "/domains/Praxis/s_centre1"]) );
    set_property("no castle", 1);
    set_property("no attack", 1);
}

void reset() {
    ::reset();
    if(!present("list")) 
	new("std/obj/player_list")->move(this_object());
    if(!present("lars")) 
	new("/domains/Praxis/obj/mon/lars")->move(this_object());
}

int read(string str) {
    object ob;
    int i;

    ob = present("lars");
    if(!ob) {
	write("You cannot read the menu, as it is splattered with blood.");
	return 1;
    }
    message("Ninfo", "The following classic drinks are served at the Lars Pub!\n", this_player());
    message("Ninfo", "-----------------------------------------------------------\n", this_player());
    message("Ninfo", "A firebreather\t\t\t"+ (int)ob->get_price("firebreather")+" gold\n", this_player());
    message("Ninfo", "A special of the house\t\t"+(int)ob->get_price("special")+" gold\n", this_player());
    message("Ninfo", "A pale ale\t\t\t"+(int)ob->get_price("ale")+" gold\n", this_player());
    message("Ninfo", "-----------------------------------------------------------\n", this_player());
    message("Ninfo", "<buy drink_name> gets you a drink.\n", this_player());
    return 1;
}









