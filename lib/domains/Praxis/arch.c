inherit "/lib/std/room";

void init() {
    ::init();
    if(!archp(this_player()))
        this_player()->eventMoveLiving("/domains/Praxis/planning_room", "down");
}

void create() {
    object ob;

    ::create();
    SetProperty("light", 2);
    SetProperty("indoors", 1);
    SetShort( "The Arch's Room");
    SetLong(
            "This is where the arches decide the fate of Nightmare.");
    SetExits( 
            (["down" : "/domains/Praxis/planning_room"]) );

    ob = new("/lib/bboard");
    ob->SetKeyName("board");
    ob->SetId( ({ "board" }) );
    ob->set_board_id("arch");
    ob->set_max_posts(20);
    ob->move("/domains/Praxis/arch");
    ob->SetShort( "the Arch Immortal's Board");
    ob->SetLong( "A board for utter nonsense.\n");
}

