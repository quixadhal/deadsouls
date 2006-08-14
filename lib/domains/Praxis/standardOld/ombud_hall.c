inherit "std/room";

void create() {
    object ob;

    ::create();
    set_property("light", 2);
    set_property("indoors", 1);
    set_short( "Complaint Department");
    set_long(
      "You are in a dark, musty room.  This is the official "
      "Nightmare complaints department.  Post any gripes, questions, or comments "
      "you have that you wish for the wizards to address on the board.");
    set_exits( 
      ([
	"up" : "/domains/Praxis/adv_main",
	"west" : "/domains/Praxis/hm_chamber.c"
      ]) 
    );

    ob = new("std/bboard");
    ob->set_name("board");
    ob->set_id( ({ "comments board", "board" }) );
    ob->set_board_id("ombud");
    ob->set_max_posts(50);
    ob->set_edit_ok( ({ "nialson", "ninja" }) );
    ob->set_short("the Board of the Complainers");
    ob->set_long( "People of this reality come here to post their "
      "comments and questions for the immortals who control "
      "the fate of all reality.\n");
    ob->move(this_object());
    set_property("no steal", 1);
    set_property("no attack", 1);
    set_property("no castle", 1);
}

