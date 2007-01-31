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
	"/domains/town/obj/maglite" : 3,
	"/domains/town/obj/watch" : 10,
	"/domains/town/obj/pack" : 10,
	"/domains/town/weap/dagger" : 30,
	"/domains/town/weap/verysharpsword" : 2,
	"/domains/town/armor/chainmail" : 3,
	"/domains/town/obj/flask" : 10,
	"/domains/town/obj/dcell_std" : 30,
	"/domains/town/weap/sword" : 30,
	"/domains/town/obj/match" : 15,
	"/domains/town/obj/torch" : 15,
	"/domains/town/armor/helmet" : 3,
	"/domains/town/obj/rayovac" : 10,
	"/domains/town/obj/canteen" : 5,
      ]));
    SetNoClean(1);

    SetExits( ([
	"south" : "/domains/town/room/shop",
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
void init(){
    ::init();
}
