#include <lib.h>

inherit LIB_ROOM;

static void create() {
    object ob;
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("Test Labs Corridor");
    SetLong("This is the main corridor of the test labs. North is the combat arena. South is the menagerie. The Creators Hall is west of here, and the corridor continues east.");
    SetExits( ([
	"south" : "/domains/default/room/wiz_corr_south",
	"west" : "/domains/default/room/wiz_hall",
	"north" : "/domains/default/room/arena",
	"east" : "/domains/default/room/wiz_corr_east.c",
      ]) );
    SetInventory(([
      ]));

    SetProperty("no attack", 1);

    SetDoor("north", "/domains/default/doors/steel_door2.c");

}
int CanReceive(object ob) {
    return room::CanReceive();
}

void init(){
    ::init();
}
