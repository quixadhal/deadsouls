#include <lib.h>

inherit LIB_ROOM;

static void create() {
    object ob;
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("Disease Lab");
    SetLong("This room is designed for testing the effects and transmissibility of disease. Another testing area is east.");
    SetExits( ([ 
                "east" : "/domains/default/room/quarantine3",
                "up" : "/domains/default/room/quarantine1",
                ]) );
    SetNoClean(1);
    SetInventory(([
                "/domains/default/npc/quarantine/wim" : 1,
                "/domains/default/npc/quarantine/rik" : 1,
                "/domains/default/npc/quarantine/han" : 1,
                "/domains/default/npc/quarantine/tom" : 1,
                "/domains/default/npc/quarantine/lex" : 1,
                "/domains/default/npc/quarantine/jan" : 1,
                "/domains/default/npc/quarantine/ted" : 1,
                "/domains/default/npc/quarantine/pip" : 1,
                "/domains/default/npc/quarantine/cor" : 1,
                "/domains/default/npc/quarantine/kip" : 1,
                "/domains/default/npc/quarantine/job" : 1,
                "/domains/default/npc/quarantine/ton" : 1
                ]));
    AddItem(new("/domains/default/obj/reset_button"));

}
int CanReceive(object ob) {
    return room::CanReceive();
}
void init(){
    ::init();
}
