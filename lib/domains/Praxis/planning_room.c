inherit "/lib/std/room";

void create() {
    object ob;

    ::create();
    SetProperty("light", 2);
    SetProperty("indoors", 1);
    SetShort( "The Planning Room");
    SetLong( (: this_object(), "go_away" :));
    SetExits( 
            (["west" : "/domains/Praxis/adv_inner",
             ]) );
    AddExit("up", "/domains/Praxis/arch", (:"do_check":));
    ob = new("/lib/bboard");
    ob->SetKeyName("board");
    ob->SetId( ({ "board", "planning board" }) );
    ob->set_board_id("planning");
    ob->set_max_posts(30);
    ob->move("/domains/Praxis/planning_room");
    ob->SetShort( "the Planning Our Reality Board");
    ob->SetLong( "A board for posting ideas so that others "
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
void init(){
    ::init();
}
