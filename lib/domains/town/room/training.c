#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("the training area");
    SetLong("You are in a large room with a very high ceiling, mirrored walls, and a hardwood floor. This area is designed for skills training, though at the moment no workshops are scheduled. The rest of the Adventurers' Guild is downstairs.");
    SetItems( ([
                ({"wall","walls"}) : "The walls have mirrors set into them.",
                ({"mirror","mirrors"}) : "Peekaboo!",
                ({"hardwoord floor", "floor"}) : "The floor is made of polished wood, and is "+
                "reminiscent of what you might expect in a dancing school.",
                ({"ceiling","high ceiling"}) : "The high, vaulted ceiling "+
                "provides plenty of headroom." 
                ]) );
    SetExits( ([ 
                "down" : "/domains/town/room/adv_guild",
                ]) );
    SetInventory(([
                "/domains/town/npc/radagast" : ({60, 1}),
                "/domains/town/obj/bbucket" : 1,
                ]));
    SetPlayerKill(1);
}
void init(){
    ::init();
}
