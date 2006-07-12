#include <lib.h>
inherit LIB_NPC;

static void create(){
    npc::create();
    SetKeyName("fighter");
    SetId( ({"human","npc","warrior","fighter"}) );
    SetShort("a fighter");
    SetLong("This is a large human warrior. His pectoral muscles "+
      "are clearly visible even through his armor. His face is covered in "+
      "bold blue tattoos.");
    SetInventory(([
	"/domains/default/armor/chainmail.c" : "wear chainmail",
	"/domains/town/weap/sword" : "wield sword",
      ]));
    SetLevel(10);
    SetRace("human");
    SetClass("fighter");
    SetGender("male");
    SetMaxHealthPoints(550);
}
void init(){
    ::init();
}
