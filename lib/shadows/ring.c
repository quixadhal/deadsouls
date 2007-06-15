#include <damage_types.h>
inherit LIB_SHADOW;
int reporting, protecting;

varargs int eventReceiveDamage(object agent, int type, int x, int internal, mixed limbs) {
    int stamina, fatigue, hp, damage, damdiff;
    string evidence, limb_string;
    if(reporting){
	evidence = "%^BOLD%^%^RED%^";
	if(agent) evidence += "You receive damage from "+agent->GetKeyName();
	if(type) {
	    switch(type){
	    case BLUNT : evidence += ", damage type is BLUNT";break; 
	    case BLADE : evidence += ", damage type is BLADE";break;
	    case KNIFE : evidence += ", damage type is KNIFE";break;
	    case WATER : evidence += ", damage type is WATER";break;
	    case SHOCK : evidence += ", damage type is SHOCK";break;
	    case COLD : evidence += ", damage type is COLD";break;
	    case HEAT : evidence += ", damage type is HEAT";break;
	    case GAS : evidence += ", damage type is GAS";break;
	    case ACID : evidence += ", damage type is ACID";break;
	    case MAGIC : evidence += ", damage type is MAGIC";break;
	    case POISON : evidence += ", damage type is POISON";break;
	    case DISEASE : evidence += ", damage type is DISEASE";break;
	    case TRAUMA : evidence += ", damage type is TRAUMA";break;
	    case PIERCE : evidence += ", damage type is PIERCE";break;
	    case PSIONIC : evidence += ", damage type is PSIONIC";break;
	    case ANOXIA : evidence += ", damage type is ANOXIA";break;
	    case DEATHRAY : evidence += ", damage type is DEATHRAY";break;
	    case EMOTIONAL : evidence += ", damage type is EMOTIONAL";break;
	    case SONIC : evidence += ", damage type is SONIC";break;
	    case BITE : evidence += ", damage type is BITE";break;
	    case OTHER : evidence += ", damage type is OTHER";break;
	    default : evidence += ", damage type is UNKNOWN";break;
	    }
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
	else limb_string = ", and you can't tell where you're hit. ";
	if(limbs) { 
	    evidence += ", body part(s) affected: ";
	    evidence += limb_string + ".";
	}
	this_object()->eventPrint(evidence+"%^RESET%^");
	hp = this_object()->GetHealthPoints();
	stamina =  this_object()->GetStaminaPoints();

	if(!agent) agent = this_object();
	if(!type) type = 0;
	if(!x) x = 0;
	if(!internal) internal = 0;
	if(!limbs) limbs = "";

	if(x > this_object()->GetHealthPoints()){
	    this_object()->eventPrint("%^RED%^This could have been a fatal hit. "+
	      "The ring's safety protocol prevents \"actual damage\" calculation "+
	      "from proceeding.%^RESET%^");
	    return 1;
	}

	GetShadowedObject()->eventReceiveDamage(agent, type, x, internal, limbs);

	damage = this_object()->GetHealthPoints();
	fatigue = stamina - this_object()->GetStaminaPoints();
	damdiff = hp - damage;
	this_object()->eventPrint("%^RED%^Actual damage done: "+damdiff+"%^RESET%^");
	this_object()->eventPrint("%^YELLOW%^Stamina sapped: "+fatigue+"%^RESET%^");
	if(protecting){
	    GetShadowedObject()->AddHP(damdiff+1);
	    this_object()->AddStaminaPoints(fatigue+1);
	}
    }
    else if(!protecting) 
	return GetShadowedObject()->eventReceiveDamage(agent, type, x, internal, limbs);
    return 1;
}

int RemoveLimb(string limb, object agent){
    if(protecting){
	this_object()->eventPrint("you have received enough damage to sever it. "+
	  "However, The ring's protection prevents that.");
	return 1;
    }
    else return GetShadowedObject()->RemoveLimb(limb, agent);
}

int AddHP(int hp){
    if(protecting && hp < 0){
	if(reporting)
	    this_object()->eventPrint(identify(previous_object())+" tried to "+
	      "deduct "+abs(hp)+" health. The ring's protection prevents it.");
	return 1;
    }
    if(reporting){
	string operation = "add";
	if(hp < 0) operation = "subtract";
	this_object()->eventPrint(identify(previous_object())+" tried to "+
	  operation+" "+abs(hp)+" health.");
    }
    return GetShadowedObject()->AddHP(hp);
}

int JadeProtection(int i){
    if(!this_player()) return 0;
    if(!(this_player() == this_object()->GetShadowedObject())) return 0;
    protecting = i;
    return protecting;
}

int JadeReporting(int i){
    if(!this_player()) return 0;
    if(!(this_player() == this_object()->GetShadowedObject())) return 0;
    reporting = i;
    return reporting;
}

int eventUnshadow(){
    if(!this_player()) return 0;
    if(!(this_player() == this_object()->GetShadowedObject())) return 0;
    return ::eventUnshadow();
}
