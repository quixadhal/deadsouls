inherit "/std/room";

void init() {
    ::init();
    if(!archp(this_player()))
	this_player()->move_player("/domains/Praxis/planning_room", "down");
}

void create() {
    object ob;

    ::create();
    set_property("light", 2);
    set_property("indoors", 1);
    set_short( "The Arch's Room");
    set_long(
      "This is where the arches decide the fate of Nightmare.");
    set_exits( 
      (["down" : "/domains/Praxis/planning_room"]) );

    ob = new("/std/bboard");
    ob->set_name("board");
    ob->set_id( ({ "board" }) );
    ob->set_board_id("arch");
    ob->set_max_posts(20);
    ob->move("/domains/Praxis/arch");
    ob->set_short( "the Arch Immortal's Board");
    ob->set_long( "A board for utter nonsense.\n");
}


