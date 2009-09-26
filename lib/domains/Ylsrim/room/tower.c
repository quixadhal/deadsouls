/*    /domains/Ylsrim/room/tower.c
 *    from the Dead Souls Mud Library
 *    created by Descartes of Borg 960512 
 */ 

#include <lib.h> 
#include <jump.h> 

inherit LIB_ROOM; 
inherit LIB_JUMP; 

int ReadSign() {
    this_player()->more("/domains/town/txt/ylsrim_sign.txt");
    return 1;
}

void create() { 
    room::create();
    SetClimate("indoors");
    SetTown("Ylsrim");
    SetAmbientLight(28);
    SetShort("Ylsrim Tower");
    SetLong("The tower looks out over Kaliid Road.  South of the road is "
            "the always busy Ylsrim bazaar.  A spiraling staircase leads "
            "down into the heart of the adventurer's hall."
            "\n%^RED%^There is a sign here you can read.%^RESET%^");
    SetItems( ([ ({ "road", "kaliid road" }) :
                "You could probably jump down into the road from here.",
                "bazaar" : "It looks like it is bustling.",
                "sign":"A sign. To read it, 'read sign'.",
                "tower" : "The tower you are on.",
                ({ "stairs", "staircase" }) : "They lead down into the "
                "heart of the hall." ]) );
    SetExits( ([ "down" : "/domains/Ylsrim/room/"+ "adv_hall" ]) );
    SetInventory(([
                "/domains/Ylsrim/obj/stargate" : 1,
                ]));
    SetRead("sign", (: ReadSign :) );
    SetCoordinates("1000,1001,1");
} 
void init(){
    ::init();
}
