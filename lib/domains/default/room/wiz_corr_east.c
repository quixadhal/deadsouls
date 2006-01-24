#include <lib.h>

inherit LIB_ROOM;

static void create() {
    object ob;
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("Test Labs Corridor East");
    SetLong("This is the main corridor of the test labs. North is the disease lab. The corridor runs west from here.");
    SetExits( ([
	"west" : "/domains/default/room/wiz_corr1",
	"north" : "/domains/default/room/quarantine1.c",
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
