#include <lib.h>

inherit LIB_ROOM;

static void create() {
    object ob;
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("Test Labs Corridor East");
    SetLong("This is the main corridor of the test labs. The corridor runs east and west. South is the stargate lab.");
    SetExits( ([
                "east" : "/domains/default/room/wiz_corr_east3",
                "west" : "/domains/default/room/wiz_corr_east",
                "south" : "/domains/default/room/stargate_lab.c",
                ]) );
    SetProperty("no attack", 1);
}

int CanReceive(object ob) {
    return room::CanReceive();
}

void init(){
    ::init();
}
