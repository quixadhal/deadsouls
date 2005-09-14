#include <lib.h>
inherit LIB_ROOM;
static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("Otik's storage room");
    SetLong("This is a small, bare room where Otik "
		    "keeps his goods. The main store "
		    "is south.");
    SetInventory(([
		"/domains/default/obj/torch" : 15,
		"/domains/default/obj/match" : 15,
		"/domains/default/weap/axe" : 30,
		"/domains/default/armor/robe" : 3,
		]));

    SetExits( ([
			    "south" : "/domains/default/room/shop",
			    ]) );
    SetObviousExits("s");
}
int CanReceive(object ob) {
    if(playerp(ob) && !creatorp(ob) && !present("testchar badge",ob)) {
     message("info","Otik's back room is for authorized personnel only.", ob); 
           return 0;
}
    return 1;
    }


