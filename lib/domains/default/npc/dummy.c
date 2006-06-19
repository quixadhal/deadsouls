#include <lib.h>
#include <damage_types.h>
inherit LIB_NPC;

static void create(){
    npc::create();
    SetKeyName("dummy");
    SetId( ({"dummy","mokujin"}) );
    SetShort("a training dummy");
    SetLong("This is a magical sparring partner. It is made of "+
      "logs, cut to the proportions of a human's "+
      "head, torso, and limbs. The logs are held "+
      "together by joints made of chains.");
    SetPacifist(1);
    SetBodyComposition("wood");
    SetInventory(([
      ]));
    SetLevel(10);
    SetRace("golem");
    SetNativeLanguage("english");
    SetClass("fighter");
    SetGender("neuter");
    SetMaxHealthPoints(9000);
    SetHealthPoints(9000);
}
varargs int eventReceiveDamage(object agent, int type, int x, int internal, mixed limbs) {
    int hp, damage, damdiff;
    string evidence, limb_string;
    evidence = "";
    if(agent) evidence += "I receive damage from "+agent->GetKeyName();
    if(type) {
	if(type == BLUNT ) evidence += ", damage type is BLUNT"; 
	if(type == BLADE ) evidence += ", damage type is BLADE";
	if(type == KNIFE ) evidence += ", damage type is KNIFE";
	if(type == WATER ) evidence += ", damage type is WATER";
	if(type == SHOCK ) evidence += ", damage type is SHOCK";
	if(type == COLD ) evidence += ", damage type is COLD";
	if(type == HEAT ) evidence += ", damage type is HEAT";
	if(type == GAS ) evidence += ", damage type is GAS";
	if(type == ACID ) evidence += ", damage type is ACID";
	if(type == MAGIC ) evidence += ", damage type is MAGIC";
	if(type == POISON ) evidence += ", damage type is POISON";
	if(type == DISEASE ) evidence += ", damage type is DISEASE";
	if(type == TRAUMA ) evidence += ", damage type is TRAUMA";
	//else evidence += ", damage type is indeterminate";
    }
    if(x) evidence += ", raw damage is "+x;
    if(internal) evidence += ", internal variable is "+internal;
    if(limbs) {
	if(stringp(limbs)) limb_string = limbs;
	else if(arrayp(limbs)) {
	    if(stringp(limbs[0])) limb_string = implode(limbs,", ");
	    else if(objectp(limbs[0])){
		foreach(object limb in limbs){
		    limb_string += limb->GetKeyName()+", ";
		}
	    }
	}
    }
    else limb_string = ", and I can't tell where I'm hit. ";
    if(limbs) { 
	evidence += ", body part(s) affected: ";
	evidence += limb_string + ".";
    }
    eventForce("say "+evidence);
    hp = GetHealthPoints();

    if(!agent) agent = this_object();
    if(!type) type = 0;
    if(!x) x = 0;
    if(!internal) internal = 0;
    if(!limbs) limbs = "";

    ::eventReceiveDamage(agent, type, x, internal, limbs);

    damage = GetHealthPoints();
    damdiff = hp - damage;
    eventForce("say actual damage done: "+damdiff);
    AddHealthPoints(damdiff+1);
}
void init(){
    ::init();
}
