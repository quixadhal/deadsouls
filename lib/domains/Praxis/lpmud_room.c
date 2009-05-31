#include <lib.h>

inherit LIB_ROOM;

void create() {
    object ob;

    ::create();
    SetProperty("light", 2);
    SetProperty("indoors", 1);
    SetShort( "The LPMud Room");
    SetLong(
            "Advertisements for new LPMuds are posted here. "
            "Ads on other boards will be quickly removed.");
    SetExits( 
            (["west" : "/domains/Praxis/adv_main"]) );

    ob = new("/lib/bboard");
    ob->SetKeyName("board");
    ob->SetId( ({ "board", "lpmud board" }) );
    ob->set_board_id("lpmud");
    ob->set_max_posts(20);
    ob->move("/domains/Praxis/lpmud_room");
    ob->SetShort( "LPMud Advertising Board");
    ob->SetLong( "This board exists to promote LPMuds everywhere. "
            "Please feel free to post about a mud you know of here.\n");
}

void init(){
    ::init();
} 
