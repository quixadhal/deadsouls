#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("The Magic Shop Storeroom");
    SetLong("This is the storeroom where magical items are kept."); 
    SetInventory(([
        "/domains/town/meals/potion_strength" : 5,
        "/domains/town/meals/potion_bigheal" : 2,
        "/domains/town/obj/omni" : 1,
        "/domains/town/obj/8ball" : 3,
        "/domains/town/meals/potion_healing" : 10,
      ]));
    SetExits( ([
        "east" : "/domains/town/room/magic_shop",
      ]) );
    SetObviousExits("e");
}
int CanReceive(object ob) {
    if( playerp(ob) && !creatorp(ob)  && !present("testchar badge",ob) ) {
        message("info","The storeroom is for authorized personnel only.",ob);
        return 0;
    }
    return 1;
}
void init(){
    ::init();
}
