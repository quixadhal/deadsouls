//High Mortal Meeting Room written by Lassondra of Sunshine
//with help from that Borg geek 5/8/94
inherit "/lib/std/room";

int CanReceive(object ob) {
    if(!userp(ob) || high_mortalp(ob) || creatorp (ob)) return 1;
    write( "You cannot go in there!");
    return 0;
}

void init() {
    ::init();
    add_action( "sit", "sit" );
}

void create(){
    object ob;
    ::create();
    SetProperties( ([ "light" : 3, "no steal" : 1, "no magic" : 1,
                "no teleport" : 1, "no attack" : 1, "no castle" : 1, "no bump" : 1 ]) ); 
    SetShort( "the High Mortal Chamber" );
    SetLong( "You have entered the forum for the High Mortals of the "
            "Nightmare reality. You see a large round table standing in the middle "
            "of the room, surrounded by oaken chairs. %^BLUE%^Blue%^RESET%^ carpet "
            "is on the floor, and the walls have been painted a cream colour. "
           );
    SetItems( ([ 
                "table" : "A large, round oaken table.",
                "chairs" : "The chairs match the wood of the table.",
                "carpet" : "The carpet is a deep blue.",
                "walls" : "The walls are cream coloured."
                ]) );
    SetExits( ([ "east" : "/domains/Praxis/ombud_hall.c" ]) );

    ob = new("/lib/bboard");
    ob->SetKeyName("board");
    ob->SetId( ({ "board", "high mortal board", "bulletin board"}) );
    ob->set_board_id("hm_board");
    ob->set_max_posts(30);
    ob->set_edit_ok( ({"lassondra"}) );
    ob->set_location("/domains/Praxis/hm_chamber");
    ob->SetShort( "the High Mortal board of wisdom");
    ob->SetLong( "This is the High Mortals board of wisdom. It "+
            "is used for the High Mortals to discuss issues "+
            "concerning the welfare of the mud. ");
}

int sit(string str) {
    if(!str || str!="chair" ) {
        notify_fail( "Sit where\n?");
        return 0;
    }

    write( "You sit down in the oaken chair.");
    return 1;
}
