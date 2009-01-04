#include <lib.h>

inherit LIB_NPC;
object gdude;


int nasty(object dude){
    if(!dude) return 0;
    gdude = dude;
    eventForce("look at "+gdude->GetKeyName());
    eventForce("drool");
    return 1;
}

int CheckHuman(mixed val){
    if(!val) return 0;
    if(!objectp(val)) return 0;
    gdude = val;
    if(val->GetRace() != "human") return 0;
    else { 
        call_out( (: nasty, gdude :) , 2);
    }
    return 1;
}
static void create() {
    npc::create();
    SetKeyName("troll");
    SetId(({"troll"}));
    SetAdjectives(({"dirty"}));
    SetShort("a mean-looking troll");
    SetLong("This is the dreaded creature of legend: a slimy, "
            "green, putrid-looking fiend with long, razor-"
            "sharp claws and fangs.");
    SetLevel(3);
    SetRace("troll");
    SetClass("fighter");
    SetGender("male");
    SetMelee(1);
    SetEncounter(  (: CheckHuman :) );
    SetSkill("melee attack",50,50,10);
    SetStat("strength",50,1);
    SetStat("agility",50,1);
    SetStat("coordination",50,1);
    SetStat("speed",50,1);
    AddCurrency("silver",221+random(157));
}
void init(){
    ::init();
}
