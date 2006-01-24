#include <lib.h>

inherit LIB_ROOM;

static void create() {
    object ob;
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("The Menagerie");
    SetLong("There are more creatures to the south, east, and west.");
    SetInventory(([
	"/domains/default/npc/horse" : 1,
	"/domains/default/npc/bird" : 1,
	"/domains/default/npc/elephant" : 1,
	"/domains/default/npc/bat" : 1,
	"/domains/default/npc/newt" : 1,
	"/domains/default/npc/lynx" : 1,
	"/domains/default/npc/deer" : 1,
	"/domains/default/npc/spider" : 1,
	"/domains/default/npc/bear" : 1,
	"/domains/default/npc/dryad" : 1,
	"/domains/default/npc/rat" : 1,
	"/domains/default/npc/zookeeper" : 1,
	"/domains/default/npc/cow" : 1,
	"/domains/default/npc/centaur" : 1,
	"/domains/default/npc/gnome" : 1,
	"/domains/default/npc/moth" : 1,
	"/domains/default/npc/gecko" : 1,
      ]));
    SetExits( ([
	"south" : "/domains/default/room/menagerie_south",
	"north" : "/domains/default/room/wiz_corr_south",
	"east" : "/domains/default/room/menagerie_e",
	"west" : "/domains/default/room/menagerie_w.c",
      ]) );

}
void init(){
    ::init();
}
