#include <lib.h>

inherit LIB_ROOM;

static void create() {
    object ob;
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("Test Labs Corridor West");
    SetLong("This is the west end of main corridor of the test labs. North is the disease lab. The Creators Hall is west of here, and corridor continues east.");
    SetExits( ([ 
                "north" : "/domains/default/room/quarantine1",
                "east" : "/domains/default/room/wiz_corr_east",
                "west" : "/domains/default/room/wiz_hall",
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
