#include <lib.h>
#include "../../customdefs.h"

inherit LIB_NPC;

static void create(){
    npc::create();
    SetKeyName("fighter");
    SetId( ({"human","npc","warrior","fighter"}) );
    SetShort("a fighter");
    SetLong("This is a sample warrior NPC.");
    SetLevel(10);
    SetRace("human");
    SetClass("fighter");
    SetGender("male");
    SetInventory(([
	MY_ARMOR "/chainmail.c" : "wear chainmail",
	MY_ARMOR "/leather_boot_r.c" : "wear boot",
	MY_ARMOR "/leather_boot_l.c" : "wear boot",
	MY_WEAP "/sword.c" : "wield sword",
      ]));
}
void init(){
::init();
}