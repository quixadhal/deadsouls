#include <lib.h>
#include "../customdefs.h"
inherit LIB_SENTIENT;
inherit MY_AREA + "/etc/call_cops";

static void create(){
    sentient::create();
    SetKeyName("guard");
    SetId( ({"human","gate guard","warrior","soldier"}) );
    SetShort("a guard");
    SetLong("This is a large professional soldier. He wears "+
      "the proud crimson insignia of the Fortress Guards, a legendary "+
      "force of warriors.");
    SetInventory(([
	"/domains/fort/armor/leather_boot_r.c" : "wear boot",
	"/domains/fort/armor/chainmail.c" : "wear chainmail",
	"/domains/fort/armor/platemail_pants.c" : "wear pants",
	"/domains/fort/armor/steel_helmet.c" : "wear helmet",
	"/domains/fort/armor/shield.c" : "wear shield on left arm",
	"/domains/fort/armor/steel_glove_l.c" : "wear glove",
	"/domains/fort/armor/steel_glove_r.c" : "wear glove",
	"/domains/fort/armor/crimson_cloak.c" : "wear cloak",
	"/domains/fort/armor/leather_boot_l.c" : "wear boot",
      ]));
    SetLevel(5);
    SetRace("human");
    SetClass("fighter");
    SetGender("male");
    SetAutoStand(0);
}
void init(){
    ::init();
}
