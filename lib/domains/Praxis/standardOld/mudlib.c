inherit "/std/room";

void create() {
    object ob;

    ::create();
    set_property("light", 2);
    set_property("indoors", 1);
    set_short( "The Mudlib Room");
    set_long(
      "In this room you can see the status of work being done on "
      "bugs in the game by the mudlib department, as well as report "
      "bugs that have not been getting the attention you think the bug "
      "deserves.");
    set_exits( 
      (["down" : "/domains/Praxis/adv_inner"]) );

    ob = new("/std/bboard");
    ob->set_name("board");
    ob->set_id( ({ "board" }) );
    ob->set_board_id("mudlib");
    ob->set_max_posts(50);
    ob->move("/domains/Praxis/mudlib");
    ob->set_short( "the Mudlib Board");
    ob->set_long( "A board for complete and utter nonsense.\n");
}


