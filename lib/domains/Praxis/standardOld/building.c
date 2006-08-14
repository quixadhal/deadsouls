#include <std.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 1);
    set_property("indoors", 1);
    set_smell_string("default", "The building smells stale and decaying.");
    set_listen_string("default", "The boards which make up "
      "the floor are creaking under your weight.");
    set_short( "a broken down building");
    set("long",
      "Broken boards and rotting thatch have left this place very "
      "dangerous for walking.  The squeaks of diseased rodents "
      "tell you that this place is no longer run by humanoids, but "
      "bits of evidence here and there suggest recent humanoid passage. "
      "The alley lies north of here.");
    set_items(
      (["board" : "It is rotting from exposure to the weather.",
	"boards" : "You do not want to rely on them for support.",
	"thatch" : "It once formed the roof of the building, but no more.",
	"roof" : "A patchwork of rotting thatch.",
	"evidence" : "Dust left unsettled.",
	"dust" : "It looks like it has not been very long settled in places.",
	"foo" : "What do you mean by foo?"]) );
    set_exits( 
      (["north" : "/d/Praxis/alley2"]) );
}

void reset() {
    object rat;

    ::reset();
    if(!present("rat")) {
	rat = new("std/monster");
	rat->set_name("rat");
	rat->set_id( ({ "rat", "filthy rat", "a filthy rat" }) );
	rat->set_level(1);
	rat->set("short", "A filthy rat");
	rat->set("long", "A disgusting little rodent.");
	rat->set("race", "rodent");
	rat->set_body_type("quadruped");
	rat->set_money("silver", random(10));
	rat->set_hp(60+random(10));
	rat->set_alignment(-1);
	rat->move(this_object());
    }
    if(!present("torch")) 
	new("std/obj/torch")->move(this_object());
}

