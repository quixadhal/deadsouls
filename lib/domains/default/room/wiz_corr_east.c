#include <lib.h>

inherit LIB_ROOM;

static void create() {
    object ob;
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("Test Labs Corridor");
    SetLong("This is the main corridor of the test labs. North is the disease lab. The corridor runs east and west from here. South is the new Stargate lab.");
    SetExits( ([
        "north" : "/domains/default/room/quarantine1",
        "south" : "/domains/default/room/stargate_lab",
        "west" : "/domains/default/room/wiz_corr1",
        "east" : "/domains/default/room/wiz_corr_east2.c",
      ]) );

    SetInventory(([
      ]));

    SetProperty("no attack", 1);

}
int CanReceive(object ob) {
    return room::CanReceive();
}

void init(){
    ::init();
}
