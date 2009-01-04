#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(15);
    SetShort("The Orc Fortress");
    SetLong("You are deep in the orc stronghold. Like the rest of the fortress you have seen, this looks hastily put together and rather clumsily built, with poor maintenance besides. This is some kind of ceremonial chamber or temple. Dried blood surrounds a raised altar, and skulls of various types adorn the walls. The rest of the fort is east of here.");
    SetItems(([
                ({ "skull","skulls" }) : "You see some skulls whose race you "+
                "recognize, but most you don't. There is at least one "+
                "human skull here. Some of them are so fresh they are "+
                "still wet and have tissue hanging from them.",
                ({"chamber","ceremonial chamber","temple"}) : "A place "+
                "where orcish rituals are performed.",
                ({"blood","dried blood"}) : "Evidently the ground around "+
                "the altar has collected blood from whatever happens on "+
                "the altar.",
                ({"wall","walls"}) : "Rough-hewn walls of earth and wood.",
                ]) );
    SetExits( ([ 
                "east" : "/domains/town/room/orc_fortress",
                ]) );
    SetSmell( ([ "default" : "The stench of garbage and animal waste hangs here."]) );
    SetInventory(([
                "/domains/town/obj/altar" : 1,
                "/domains/town/npc/orc_shaman" : 1,
                ]));

}
void init(){
    ::init();
}
