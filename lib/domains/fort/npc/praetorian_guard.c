#include <lib.h>
#include "/realms/cratylus/customdefs.h"

inherit LIB_SENTIENT;

static void create(){
    sentient::create();
    SetKeyName("praetorian");
    SetId( ({"soldier","human","praetorian guard","warrior","guard"}) );
    SetShort("a soldier of the Praetorian Guard");
    SetLong("This is an elite soldier. He wears "+
      "the proud crimson helmet of the Fortress Praetorian "+
      "Guard. The warriors of the Praetorian Guard are "+
      "said to be hand-picked by the Castellan for their "+
      "strength, bravery and skill. They form his personal "+
      "protection force.");
    SetWanderSpeed(20);
    SetLevel(10);
    SetRace("human");
    SetClass("fighter");
    SetGender("male");
    SetMaxCarry(4000);
    SetInventory(([
	MY_ARMOR "/platemail.c" : "wear platemail",
	MY_ARMOR "/steel_boot_r.c" : "wear boots",
	MY_ARMOR "/steel_boot_l.c" : "wear boots",
	MY_ARMOR "/steel_glove_r.c" : "wear gloves",
	MY_ARMOR "/steel_glove_l.c" : "wear gloves",
	MY_ARMOR "/crimson_cloak.c" : "wear cloak",
	MY_ARMOR "/crimson_helmet.c" : "wear helmet",
	MY_ARMOR "/shield.c" : "wear shield on left arm",
	MY_WEAP "/std_sword.c" : "wield sword in right hand",
      ]));
}
void init(){
    ::init();
}
