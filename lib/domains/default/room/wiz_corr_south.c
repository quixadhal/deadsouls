#include <lib.h>

inherit LIB_ROOM;

static void create() {
    object ob;
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("Menagerie Corridor");
    SetLong("This short corridor leads down to the menagerie. This corridor has been specially created to repel NPC's, so that the creatures  below do not escape. The main test lab corridor is north.");
    SetExits( ([
                "north" : "/domains/default/room/wiz_corr_east",
                "down" : "/domains/default/room/menagerie.c",
                ]) );

    SetInventory(([
                ]));

}
int CanReceive(object ob) {
    //if(living(ob) && !creatorp(ob) && !present("testchar badge",ob)) {
    //	message("info","Creator staff only, sorry.", ob);
    //	return 0;
    //   }
    if(living(ob) && !interactive(ob)){
        message("info","NPC's not allowed, sorry.", ob);
        return 0;
    }
    return room::CanReceive(ob);
}

void init(){
    ::init();
}
