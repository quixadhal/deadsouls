#include <lib.h>

inherit LIB_ROOM;

static void create() {
    object ob;
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("Menagerie Annex");
    SetLong("This area was added to the menagerie to allow a little leg room for creatures that like to wander.");
    SetInventory(([
                "/domains/default/npc/chimera" : 1,
                "/domains/default/npc/ogre" : 1,
                "/domains/default/npc/orc" : 1,
                "/domains/default/npc/red_dragon" : 1,
                "/domains/default/npc/bacchus" : 1,
                "/domains/default/npc/minotaur" : 1,
                "/domains/default/npc/goblin" : 1,
                "/domains/default/npc/golem" : 1,
                "/domains/default/npc/troll" : 1,
                "/domains/default/npc/klingon" : 1,
                "/domains/default/npc/gargoyle" : 1,
                "/domains/default/npc/balrog" : 1,
                "/domains/default/npc/kobold" : 1,
                "/domains/default/npc/wraith" : 1,
                "/domains/default/npc/yattering" : 1,
                "/domains/default/npc/gnoll" : 1,
                "/domains/default/npc/elemental" : 1,
                ]));
    SetExits(([
                "north" : "/domains/default/room/menagerie",
                ]));
}
void init(){
    ::init();
}
