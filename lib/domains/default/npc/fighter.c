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
    SetLevel(10);
    SetRace("human");
    SetClass("fighter");
    SetGender("male");
    SetInventory(([
	"/domains/default/armor/chainmail.c" : "wear chainmail",
	"/domains/default/weap/sharpsword.c" : "wield sword"
      ]));
    SetMaxHealthPoints(550);
}
void init(){
    ::init();
}
