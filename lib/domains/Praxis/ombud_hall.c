inherit "/lib/std/room";

void create() {
    object ob;

    ::create();
    SetProperty("light", 2);
    SetProperty("indoors", 1);
    SetShort( "Complaint Department");
    SetLong(
            "You are in a dark, musty room.  This is the official "
            "Nightmare complaints department.  Post any gripes, questions, or comments "
            "you have that you wish for the wizards to address on the board.");
    SetExits( 
            ([
             "up" : "/domains/Praxis/adv_main",
             "west" : "/domains/Praxis/hm_chamber.c"
             ]) 
            );

    ob = new("/lib/bboard");
    ob->SetKeyName("board");
    ob->SetId( ({ "comments board", "board" }) );
    ob->set_board_id("ombud");
    ob->set_max_posts(50);
    ob->set_edit_ok( ({ "nialson", "ninja" }) );
    ob->SetShort("the Board of the Complainers");
    ob->SetLong( "People of this reality come here to post their "
            "comments and questions for the immortals who control "
            "the fate of all reality.\n");
    ob->move(this_object());
    SetProperty("no steal", 1);
    SetProperty("no attack", 1);
    SetProperty("no castle", 1);
}

void init(){
    ::init();
} 
