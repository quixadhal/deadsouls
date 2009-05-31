inherit "/lib/std/room";

void create() {
    object ob;

    ::create();
    SetProperty("light", 2);
    SetProperty("indoors", 1);
    SetShort( "The LPC and MudOS Room");
    SetLong(
            "Immortals come here to discuss coding problems.  "
            "The Hall of Immortals is north of here.");
    SetExits( 
            (["north" : "/domains/Praxis/adv_inner"]) );

    ob = new("/lib/bboard");
    ob->SetKeyName("board");
    ob->SetId( ({ "board", "irreality board" }) );
    ob->set_board_id("coding");
    ob->set_max_posts(20);
    ob->move("/domains/Praxis/lpc_inner");
    ob->SetShort( "the LPC and MudOS Irreality Board");
    ob->SetLong( "Post your questions about coding here.\n");
}
void init(){
    ::init();
}
