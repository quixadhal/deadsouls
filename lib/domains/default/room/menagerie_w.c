#include <lib.h>
#include ROOMS_H

inherit LIB_ROOM;

void create() {
    room::create();
    SetAmbientLight(30);
    SetShort("Menagerie, west");
    SetLong("This is the western annex of the menagerie.");
    SetInventory(([
                "/domains/default/npc/dwarf" : 1,
                "/domains/default/npc/helf" : 1,
                "/domains/default/npc/artrell" : 1,
                "/domains/default/npc/halfling" : 1,
                "/domains/default/npc/nymph" : 1,
                "/domains/default/npc/avidryl" : 1,
                "/domains/default/npc/faerie" : 1,
                "/domains/default/npc/hobbit" : 1,
                "/domains/default/npc/pessis" : 1,
                "/domains/default/npc/kender" : 1,
                "/domains/default/npc/zoe" : 1,
                ]));
    SetExits(([
                "east" : "/domains/default/room/menagerie",
                ]));

}
void init(){
    ::init();
}
