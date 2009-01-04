#include <lib.h>

inherit LIB_ROOM;

static void create() {
    object ob;
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("Disease Lab Annex");
    SetLong("This room is designed for testing the effects and transmissibility of disease. Another testing area is west.");
    SetExits( ([ 
                "west" : "/domains/default/room/quarantine2",
                ]) );
    SetInventory(([
                "/domains/default/npc/quarantine/nigel" : 1,
                "/domains/default/npc/quarantine/graham" : 1,
                "/domains/default/npc/quarantine/duncan" : 1,
                "/domains/default/npc/quarantine/trevor" : 1,
                "/domains/default/npc/quarantine/simon" : 1,
                "/domains/default/npc/quarantine/spencer" : 1,
                "/domains/default/npc/quarantine/gavin" : 1,
                "/domains/default/npc/quarantine/adrian" : 1,
                "/domains/default/npc/quarantine/miles" : 1,
                "/domains/default/npc/quarantine/edmund" : 1,
                "/domains/default/npc/quarantine/wallace" : 1,
                "/domains/default/npc/quarantine/lester" : 1,
                ]));
    SetNoClean(1);
    AddItem(new("/domains/default/obj/reset_button"));
}
int CanReceive(object ob) {
    return room::CanReceive();
}
void init(){
    ::init();
}
