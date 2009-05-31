inherit "/lib/std/room";

void create() {
    object ob;

    ::create();
    SetProperty("light", 2);
    SetProperty("indoors", 1);
    SetShort( "The Mudlib Room");
    SetLong(
            "In this room you can see the status of work being done on "
            "bugs in the game by the mudlib department, as well as report "
            "bugs that have not been getting the attention you think the bug "
            "deserves.");
    SetExits( 
            (["down" : "/domains/Praxis/adv_inner"]) );

    ob = new("/lib/bboard");
    ob->SetKeyName("board");
    ob->SetId( ({ "board" }) );
    ob->set_board_id("mudlib");
    ob->set_max_posts(50);
    ob->move("/domains/Praxis/mudlib");
    ob->SetShort( "the Mudlib Board");
    ob->SetLong( "A board for complete and utter nonsense.\n");
}
void init(){
    ::init();
}

