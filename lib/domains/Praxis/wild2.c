inherit "/lib/std/room";

void init() {
    ::init();
    //add_action("enter_opening", "enter");
}

void create() {
    ::create();
    SetProperty("light", 2);
    SetShort( "The tropical wilderness");
    SetLong(
            "You are inside a jungle that thickens along the path "
            "northeast.  The path widens west as it heads towards "
            "the adventuring village of Praxis."
            // "  You also notice a small opening in the thick jungle."
           );
    SetItems(
            (["jungle" : "A wilderness area full of outlaws and "
             "mysterious things.\n",
             "village" : "The adventurer's town of Praxis.",
             "vegetation" : "It gets thicker to the east.",
             "path" : "You can see it opening up towards the village west.",
             //"opening" : "On second glance, its too small to enter"
             ]) );
    SetSkyDomain("town");
    SetExits( 
            (["northeast" : "/domains/Praxis/jungle",
             "west" : "/domains/Praxis/wild1"]) );
}

int enter_opening(string str) {
    if(!str) {
        notify_fail("Enter what?\n");
        return 0;
    }
    if(str != "opening" && str != "hole" && str != "jungle") {
        notify_fail("That is not here to be entered.\n");
        return 0;
    }
    this_player()->eventMoveLiving("/domains/Praxis/outland1", "into the jungle");
    return 1;
}

