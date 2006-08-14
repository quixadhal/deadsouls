#include <std.h>

inherit ROOM;

void create() {
    ::create();
    set_smell("default", "You nearly pass out from the stench of the place.");
    set_smell("piles", "They smell of rotten elf food.");
    set_smell("food", (: this_object(), "smell_food" :));
    set_property("light", 3);
    set_short( "Praxis dump");
    set_long(
      "The waste of an entire town accumulates here at the Praxis dump.  "
      "All around you are mounds upon mounds of trash and other stinking "
      "unidentifiable things.  A small alley leads east.");
    set_items(
      (["mounds" : "Who knows what might be in amoung that crap?",
	"dump" : "The people of Praxis bring their junk here.",
	"trash" : "Anything and everything.",
	"alley" : "It leads back to Centre Path."]) );
    set_exits( 
      (["east" : "/domains/Praxis/alley2"]) );
}

void reset() {
    ::reset();
    set_search("mound", (: this_object(), "mound_searching" :));
    set_search("mounds", (: this_object(), "mound_searching" :));
}

void mound_searching() {
    message("my_action", "You found a dagger in one of the mounds of trash!",
      this_player());
    message("other_action", (string)this_player()->query_cap_name()+
      " found a dagger in one of the mounds of trash.", this_object(),
      ({ this_player() }));
    new("/domains/Praxis/obj/weapon/dagger")->move(this_object());
    remove_search("mounds");
    remove_search("mound");
}

void smell_food(string str) {
    message("my_action", "You pass out from the stench of rotten elf food.",
      this_player());
    message("other_action", (string)this_player()->query_cap_name()+
      " passes out from the stench of rotten elf food.", this_object(),
      ({ this_player() }));
    this_player()->add_sp(-3);
    this_player()->add_hp(-3);
    this_player()->set_paralyzed(10, "You are too nauseated to move!");
}

