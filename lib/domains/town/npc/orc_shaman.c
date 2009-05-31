#include <lib.h>

inherit LIB_NPC;

int CheckOrc(mixed val){
    string *allowed_races = ({ "orc", "half-orc", "bear" });
    if(!val) return 0;
    if(!objectp(val)) return 0;
    if(member_array(val->GetRace(), allowed_races) != -1) return 0;
    else eventForce("growl at "+val->GetKeyName());
    return 1;
}

static void create() {
    npc::create();
    SetKeyName("orc shaman");
    SetId(({"orc","shaman"}));
    SetAdjectives(({"dirty"}));
    SetShort("the orc shaman");
    SetLong("This orc is different from the others you've seen. "+
            "he has an unearthly ferocity about him and "+
            "is bigger, tougher, and meaner than the others.");
    SetLevel(1);
    SetRace("orc");
    SetClass("fighter");
    SetGender("male");
    SetMaxHealthPoints(200);
    SetEncounter(  (: CheckOrc :) );
    SetInventory(([
                "/domains/town/weap/gstaff":"wield staff",
                "/domains/town/weap/orcslayer":1,
                "/domains/town/armor/orc_helmet": "wear helmet",
                "/domains/town/armor/bearskin": "wear bearskin",
                ]) );

}
void init(){
    ::init();
}
