#include <lib.h>

inherit LIB_ROOM;

static void create() {
    object ob;
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("copy of menagerie_corridor.c");
    SetLong("This short corridor leads south to the menagerie. This corridor has been specially created to repel NPC's, so that the creatures south of here do not escape. The main test lab corridor is north.");
    SetExits(([
	"south" : "/domains/fort/room/menagerie_corridor",
      ]));

    SetInventory(([
      ]));

    SetProperty("no attack", 1);
    SetProperty("nopeer",1);
    ob = new("/lib/bboard");
    ob->SetKeyName("chalkboard");
    ob->SetId( ({ "board", "chalkboard", "dusty board", "dusty chalkboard" }) );
    ob->set_board_id("immortal_board");
    ob->set_max_posts(30);
    SetShort("copy of menagerie_corridor.c");
    SetLong("This short corridor leads south to the menagerie. This corridor has been specially created to repel NPC's, so that the creatures south of here do not escape. The main test lab corridor is north.");
    ob->eventMove(this_object());

}
int CanReceive(object ob) {
    if(!creatorp(ob) && !present("testchar badge",ob)) {
	message("info","Creator staff only, sorry.", ob);
	return 0;
    }
    return 1;
}
void init(){
    ::init();
}
