#include <lib.h>

inherit LIB_ROOM;

static void create() {
    object ob;
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("Menagerie Corridor");
    SetLong("This short corridor leads south to the menagerie. This corridor has been specially created to repel NPC's, so that the creatures south of here do not escape. The main test lab corridor is north.");
    SetExits( ([
	"north" : "/domains/default/room/wiz_corr1",
	"south" : "/domains/default/room/menagerie.c",
      ]) );

    SetInventory(([
      ]));

}
int CanReceive(object ob) {
    if(living(ob) && !creatorp(ob) && !present("testchar badge",ob)) {
	message("info","Creator staff only, sorry.", ob);
	return 0;
    }
    return room::CanReceive(ob);
}

void init(){
    ::init();
}
