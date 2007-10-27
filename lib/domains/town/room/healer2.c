#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("Healer's Guild Storeroom");
    SetLong("This blank room is where the guild keeps their junk.");
    SetInventory(([
        "/domains/town/obj/slip_heal" : 100,
        "/domains/town/obj/slip_regenerate" : 10,
      ]));
    SetExits( ([
        "east" : "/domains/town/room/chamber",
      ]) );
    SetObviousExits("e");

}
int CanReceive(object ob) {
    if(playerp(ob) && !creatorp(ob) && !present("testchar badge",ob)) {
        message("info","The storeroom is for guild officers only.", ob);
        return 0;
    }
    return 1;
}
void init(){
    ::init();
}
