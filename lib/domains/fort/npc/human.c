#include <lib.h>

inherit LIB_SENTIENT;

static void create() {
    sentient::create();
    SetKeyName("greg");
    SetId( ({"human"}) );
    SetAdjectives( ({"human"}) );
    SetShort("greg");
    SetLong("This is an ordinary man, of typical size and build for a human about 30 years of age.");
    SetClass("explorer");
    SetInventory(([
	"/domains/fort/weap/beads3" : "wield beads",
	"/domains/fort/armor/peasant_pants" : "wear pants",
      ]));
    SetLevel(1);
    SetMelee(1);
    SetRace("human");
    SetGender("male");
}
void init(){
    ::init();
}
