inherit "/lib/std/room";


void create() {
    ::create();
    SetProperties( ([ "light" : 2, "no castle" : 1 ]) );
    SetShort( "a deluxe sleeping chamber" );
    SetLong( "You have entered one of the deluxe rooms of the Nightmare "
            "Inn. A large bed covered with a light blue satin comforter stands "
            "in the middle of the room. A wooden closet is off to your right, and "
            "to oaken nightstands have been placed on either side of the bed. "
            "A crystal vase filled with roses has been placed on the bureau "
            "against the back wall. To exit the room, type 'leave'.");
    SetItems( ([ ({ "rooms", "room" }) :
                "The room is one of the deluxe suites at the Nightmare Inn.",
                "bed" : "The bed is very large and comfortable looking.",
                "closet" : "The wooden closet is off in the corner.",
                "vase" : "The vase is filled with fresh roses.",
                "bureau" : "The wooden bureau has been placed against the far wall.",
                "wall" : "The wall has been recently painted a creamy white colour.",
                "nightstand" : "The nightstand have been made out of a high quality "
                "wood.",
                ]) );
    SetExits( ([
                "leave" : "/domains/Praxis/hall3",
                "east" : "/domains/Praxis/hall3.c",
                ]) );
}
void init(){
    ::init();
}
