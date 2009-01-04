inherit "/lib/std/room";

void init() {
    ::init();
    add_action( "leave", "leave" );
    add_action( "bounce", "bounce" );
}   

void create() {
    ::create();
    SetProperties( ([ "light" : 2, "no castle" : 1 ]) );
    SetShort( "a sleeping chamber" );
    SetLong( "You have entered one of the sleeping chambers of "
            "the posh Nightmare Inn. The room is in perfect order. "
            "A large bed sits in the middle of the room, surrounded "
            "by night stands on both side. A crystal lamp rests "
            "the nightstand on the right. The carpet is a lovely "
            "shade of sand, and the furniture is a deep oaken "
            "color. In order to exit the room, try 'leave'." );
    SetItems( ([ 
                ({ "room", "chambers", "chamber" }) :
                "The chamber is a very comfortable room.",
                "inn" : "The Nightmare Inn is reputed to be the best "
                "in the land.",
                "bed" : "The bed is very large and comfortable. It "
                "looks like it would be fun to bounce on.",
                ({ "night stand", "night stands" }) :
                "The night stands are very solid oaken structures.",
                "lamp" : "The lamp is a very old antique lamp that "
                "is made out of fine crystal.",
                "carpet" : "The carpet is a very soothing shade of "
                "beige.", ]) );
    SetExits( ([
                "west" : "/domains/Praxis/hall.c",
                ]) );

}

int bounce(string str) {
    if(!str || str!="bed" ) {
        notify_fail( "Bounce on what?\n");
        return 0;
    }

    write( "You bounce up and down on the bed happily.");
    return 1;
}
int leave(string str) {
    write( "You open the door to your hotel room and venture out "
            "into the hallway." );
    this_player()->eventMoveLiving( "/domains/Praxis/hall.c");
    return 1;
}
