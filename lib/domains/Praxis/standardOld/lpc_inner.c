inherit "/std/room";

void create() {
    object ob;

    ::create();
    set_property("light", 2);
    set_property("indoors", 1);
    set_short( "The LPC and MudOS Room");
    set_long(
      "Immortals come here to discuss coding problems.  "
      "The Hall of Immortals is north of here.");
    set_exits( 
      (["north" : "/domains/Praxis/adv_inner"]) );

    ob = new("std/bboard");
    ob->set_name("board");
    ob->set_id( ({ "board", "irreality board" }) );
    ob->set_board_id("coding");
    ob->set_max_posts(20);
    ob->move("/domains/Praxis/lpc_inner");
    ob->set_short( "the LPC and MudOS Irreality Board");
    ob->set_long( "Post your questions about coding here.\n");
}

