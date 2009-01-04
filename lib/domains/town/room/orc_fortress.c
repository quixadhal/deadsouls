#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(15);
    SetShort("The Orc Fortress");
    SetLong("You find yourself in the entrance of the local orc stronghold. This place seems quite hostile to normal sensibilities, as the construction is rough, inelegant, and the upkeep is squalid. This wooden fort also appears somewhat amateurishly put together, as if done by youngsters playing at being soldiers. There is an exit to the outside south of here, and another room is visible to the west.");
    SetExits( ([
                "south" : "/domains/town/room/valley",
                "west" : "/domains/town/room/orc_temple.c",
                ]) );
    SetSmell( ([ "default" : "The stench of garbage and animal waste hangs here."]) );
    SetInventory(([
                "/domains/town/npc/orc" : 1,
                "/domains/town/npc/orc2" : 1,
                "/domains/town/npc/orc_boss" : 1,
                ]));
}
void init(){
    ::init();
}
