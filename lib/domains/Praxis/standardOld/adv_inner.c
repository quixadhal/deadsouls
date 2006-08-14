#include <config.h>

inherit "/std/room";

void create() {
    object ob;

    ::create();
    set_property("light", 2);
    set_property("indoors", 1);
    set_short( "The Hall of the Immortals");
    set_long(
      "This is the meeting %^GREEN%^green%^RESET%^ room of the immortals of Nightmare. "
      "In this room, matters of mud policy and events are discussed "
      "by all Nightmare immortals so that everyone might know what "
      "is going on.  There is a room for discussing LPC south and "
      "a planning room east where immortals post their plans for areas "
      "domains, guilds and quests. Down from here is the approval room.");
    set_exits( 
      (["up" : "/domains/Praxis/mudlib",
	"north" : "/domains/Praxis/adv_main",
	"south" : "/domains/Praxis/lpc_inner",
	"down": __DIR__ "app_room",
	"east" : "/domains/Praxis/planning_room"]) );

    ob = new("std/bboard");
    ob->set_name("board");
    ob->set_id( ({ "board", "reality board" }) );
    ob->set_board_id("immortal");
    ob->set_max_posts(30);
    ob->move("/domains/Praxis/adv_inner");
    ob->set_short( "Immortal's Reality Board");
    ob->set_long( "Posts any notes concerning the mud which do "
      "not belong out in a public forum among players "
      "here for immortal discussion.\n");
    set_property("no steal", 1);
}

int receive_objects(object ob) {
    if(!living(ob)) return 1;
    if(!userp(ob)) return 0;
    if(!creatorp(ob)) {
	message("my_action", "You cannot penetrate the magic force of the "
	  "blue shield.", this_player());
	message("other_action", sprintf("%s tries to pass through the magic "
	    "blue shield, but fails.", (string)this_player()->query_cap_name()),
	  environment(this_player()), ({ this_player() }));
	return 0;
    }
    return ::receive_objects(ob);
}
