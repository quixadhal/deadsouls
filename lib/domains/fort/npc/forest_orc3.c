#include <lib.h>

inherit LIB_SENTIENT;

int CheckOrc(mixed val){
    if(!val) return 0;
    if(!objectp(val)) return 0;
    if(val->GetRace() == "orc") return 0;
    else eventForce("growl at "+val->GetKeyName());
    return 1;
}
static void create() {
    sentient::create();
    SetKeyName("orc");
    SetId(({"orc"}));
    SetAdjectives(({"armored","professional"}));
    SetShort("an armored orc");
    SetLong("This is a large adult orc. He seems to have a professional "
      "bearing, and a sense of purpose, unlike the ragged, chaotic "
      "orcs you've heard talk about. He almost appears to be on a mission.");
    SetPermitLoad(1);
    SetInventory(([
	"/domains/town/armor/orc_boot_r" : "wear right boot",
	"/domains/town/armor/leather_armor" : "wear leather armor",
	"/domains/town/armor/orc_boot_l" : "wear left boot",
	"/domains/town/weap/sword" : "wield sword",
      ]));
    SetLevel(5);
    SetRace("orc");
    SetClass("explorer");
    SetSkill("multi-hand", 10);
    SetSkill("knife attack", 17);
    SetGender("male");
}
void init(){
    ::init();
}
