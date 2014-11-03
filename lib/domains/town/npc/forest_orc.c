#include <lib.h>

inherit LIB_NPC;

int CheckOrc(mixed val){
    if(!val) return 0;
    if(!objectp(val)) return 0;
    if(val->GetRace() == "orc") return 0;
    else eventForce("growl at "+val->GetKeyName());
    if(playerp(val) && val->GetRace() != "orc") return 1;
    else return 0;
}

static void create() {
    npc::create();
    SetKeyName("orc");
    SetId(({"orc"}));
    SetAdjectives(({"armored","professional"}));
    SetShort("an armored orc");
    SetLong("This is a large adult orc. He seems to have a professional "
            "bearing, and a sense of purpose, unlike the ragged, chaotic "
            "orcs you've heard talk about. He almost appears to be on a mission.");
    SetInventory(([
                "/domains/town/weap/sword" : "wield sword",
                "/domains/town/obj/orc_rucksack" : "wear my sack",
                "/domains/town/armor/leather_armor" : "wear leather armor",
                "/domains/town/armor/orc_boot_l" : "wear left boot",
                "/domains/town/armor/orc_boot_r" : "wear right boot",
                ]));
    SetLevel(1);
    SetRace("orc");
    SetClass("explorer");
    SetGender("male");
    SetEncounter(  (: CheckOrc :) );
}

void init(){
    ::init();
}
