#include <lib.h>
#include "/domains/fort/customdefs.h"

inherit LIB_SENTIENT;
inherit MY_AREA + "/etc/call_cops";

static void create(){
    sentient::create();
    SetKeyName("guard");
    SetId( ({"human","tower guard","warrior","soldier"}) );
    SetShort("a guard");
    SetLong("This is a professional soldier. He wears "+
      "the proud crimson insignia of the Fortress Guards, a legendary "+
      "force of warriors.");
    SetWanderSpeed(2);
    SetLevel(2);
    SetRace("human");
    SetClass("fighter");
    SetGender("male");
    SetInventory(([
	MY_ARMOR "/chainmail.c" : "wear chainmail",
	MY_ARMOR "/leather_boot_r.c" : "wear boot",
	MY_ARMOR "/leather_boot_l.c" : "wear boot",
	MY_ARMOR "/steel_glove_l.c" : "wear glove",
	MY_ARMOR "/steel_glove_r.c" : "wear glove",
	MY_ARMOR "/steel_helmet.c" : "wear helmet",
	MY_ARMOR "/platemail_pants.c" : "wear pants",
	MY_WEAP "/std_sword.c" : "wield sword in right hand",
      ]));
}

void init(){
    ::init();
}