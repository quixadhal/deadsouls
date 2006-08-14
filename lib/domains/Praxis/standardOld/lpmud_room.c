#include <config.h>

inherit "/std/room";

void create() {
    object ob;

    ::create();
    set_property("light", 2);
    set_property("indoors", 1);
    set_short( "The LPMud Room");
    set_long(
      "Advertisements for new LPMuds are posted here. "
      "Ads on other boards will be quickly removed.");
    set_exits( 
      (["west" : "/domains/Praxis/adv_main"]) );

    ob = new("std/bboard");
    ob->set_name("board");
    ob->set_id( ({ "board", "lpmud board" }) );
    ob->set_board_id("lpmud");
    ob->set_max_posts(20);
    ob->move("/domains/Praxis/lpmud_room");
    ob->set_short( "LPMud Advertising Board");
    ob->set_long( "This board exists to promote LPMuds everywhere. "
      "Please feel free to post about a mud you know of here.\n");
}

