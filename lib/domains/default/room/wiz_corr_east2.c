#include <lib.h>

inherit LIB_ROOM;

static void create() {
    object ob;
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("Test Labs Corridor East");
    SetLong("This is the main corridor of the test labs. North is the mounted combat arena. The corridor runs west from here.");
    SetExits( ([
        "west" : "/domains/default/room/wiz_corr_east",
        "north" : "/domains/default/virtual/arena/5,1.c",
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
