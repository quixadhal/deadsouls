/*    /domains/Ylsrim/room/tower.c
 *    from the Dead Souls V Object Library
 *    created by Descartes of Borg 960512 
 */ 

#include <lib.h> 
#include <jump.h> 

inherit LIB_ROOM; 
inherit LIB_JUMP; 

void create() { 
    room::create();
    jump::create();
    SetClimate("indoors");
    SetTown("Ylsrim");
    SetAmbientLight(28);
    SetShort("Ylsrim Tower");
    SetLong("The tower looks out over Kaliid Road.  South of the road is "
	     "the always busy Ylsrim bazaar.  A spiraling staircase leads "
	     "down into the heart of the adventurer's hall.");
    SetItems( ([ ({ "road", "kaliid road" }) :
	       "You could probably jump down into the road from here.",
	       "bazaar" : "It looks like it is bustling.",
	       ({ "stairs", "staircase" }) : "They lead down into the "
	       "heart of the hall." ]) );
    SetObviousExits("d");
    SetExits( ([ "down" : __DIR__ "adv_hall" ]) );
    AddJump("road", __DIR__ "kaliid4", JUMP_INTO);
    AddJump("kaliid road", __DIR__ "kaliid4", JUMP_INTO);
    AddJump("tower", __DIR__ "kaliid4", JUMP_FROM);
} 
