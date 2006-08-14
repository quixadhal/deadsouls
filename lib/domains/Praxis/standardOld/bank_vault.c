#include <std.h>

inherit VAULT;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 1);
    set_short( "The bank vault");
    set_long(
      "The Praxis Bank keeps all cash it has on hand in this otherwise "
      "empty vault.  The door on the south end leads to the lobby.");
    set_exits( 
      (["south" : "/domains/Praxis/bank"]) );
    set_items(
      ([ "vault" : (: "look_at_vault" :) ]) );
    set_door("vault", "/domains/Praxis/bank", "south", "bank key");
}

void reset() {
    object money;

    ::reset();
    if(!present("money")) {
	money = new("/std/obj/coins");
	money->set_money("platinum", random(10));
	money->set_money("gold", random(100));
	money->set_money("electrum", random(200));
	money->move(this_object());
    }
}

void look_at_vault() {
    if(query_open("vault")) message("my_action", "It is open.", this_player());
    else message("my_action", "It is slammed shut.", this_player());
}

