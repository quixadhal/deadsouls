inherit "/std/room";

void create() {
    object ob;

    ::create();
    set_property("light", 2);
    set_property("indoors", 1);
    set_short( "The Planning Room");
    set_long( (: this_object(), "go_away" :));
    set_exits( 
      (["west" : "/domains/Praxis/adv_inner",
      ]) );
    add_exit("up", "/domains/Praxis/arch", (:"do_check":));
    ob = new("/std/bboard");
    ob->set_name("board");
    ob->set_id( ({ "board", "planning board" }) );
    ob->set_board_id("planning");
    ob->set_max_posts(30);
    ob->move("/domains/Praxis/planning_room");
    ob->set_short( "the Planning Our Reality Board");
    ob->set_long( "A board for posting ideas so that others "
      "will not use them.\n");
}

int do_check() { return archp(this_player()); }

string go_away() {
    string str;

    if(archp(this_player())) str = "The arch meeting room is upstairs. ";
    else str = "";
    str += "All ideas are unclaimed until they appear here.  "
    "If your idea is taken, and you did not post it, you have only yourself to blame.";
    return str;
}

