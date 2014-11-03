#include <lib.h>

inherit LIB_ROOM;

void init() {
    ::init();
    add_action("climb", "climb");
}

void create() {
    ::create();
    SetProperty("light", 3);
    SetProperty("night light", 1);
    SetProperty("no castle", 1);
    SetShort( "the southern end of East Road");
    SetLong(
            "Starting your journey down East Road, you can see that this is "
            "a less travelled street. Boc La Road is south from here, running "
            "straight through Praxis. Every now and then you can see a street "
            "lamp or two, in its ancient and rusted stage. The walls of the supply "
            "store border the western side of the road, and the Praxis Library, "
            "one of the biggest libraries in this realm, is to the east.");
    SetItems(
            (["road" : "East Road, which marks the far east end of Praxis.",
             "roads" : "East Road and Boc La Road.",
             "side" : "It's the supply shop.",
             "shop" : "The local adventurer's supply shop sits on Boc La "
             "and East Road.",
             "walls" : "They are nothing special.",
             "wall" : "You are fairly certain that you cannot climb them.",
             "library" : "It is open for your reading pleasure."]) );
    SetSkyDomain("town");
    SetExits( 
            (["north" : "/domains/Praxis/east_road2",
             "south" : "/domains/Praxis/e_boc_la3",
             "east"	 : "/domains/Praxis/library"]) );
    SetProperty("no castle", 1);
}

int climb(string str) {
    if(!str) {
        notify_fail("Climb what?\n");
        return 0;
    }
    if(str != "wall" && str != "walls") {
        notify_fail("That's not here for climbing.\n");
        return 0;
    }
    if(this_player()->query_stats("dexterity") < random(30)) {
        write("Ack! You slip and hurt yourself in trying!");
        say(this_player()->query_cap_name()+" slips and hurts "+
                this_player()->query_objective()+"self in trying "
                "to climb the walls.");
        this_player()->add_hp(-(random(7)));
    }
    else {
        write("You can't quite manage the climb.");
        say(this_player()->query_cap_name()+" tries to climb the shop "
                "walls and fails.");
    }
    return 1;
}
