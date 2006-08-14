
inherit "std/room";

object board;

int check_access(object who);

void create() {
    ::create();
    set_short( "Law notice room");
    set_long( "This room is where the board for Law is kept.  There seem "
      "to be so many issues to be constantly dealt with that it "
      "got difficult to keep up with them all.  Thus, this room. "
      "Hopefully this room is restricted to arches only.");
    set_property("light", 2);
    set_property("indoors", 1);
    board = new("std/bboard");
    board->set_name("board");
    board->set_id( ({ "board", "law board" }) );
    board->set_max_posts(30);
    board->set_location("/domains/Praxis/law");
    board->set_short( "Law board");
    board->set_long( "This board is used to keep track of Law issues.\n");
    board->set_board_id("law");
}

mixed init() {
    ::init();
    if(!archp(this_player()))  {
	write("Access denied.");
	this_player()->move("/domains/Praxis/square.c");
	return 1;
    }
}

int check_access(object who) {
    return ((string)this_player()->query_name() == "manny" ||
      (string)this_player()->query_name() == "darkstar" ||
      (string)this_player()->query_name() == "forlock");
}
