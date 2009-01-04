#include <damage_types.h>
inherit LIB_SHADOW;

varargs int eventReceiveDamage(mixed agent, int type, int x, int internal, mixed limbs) {
    int hp, damage, damdiff;
    object ob = GetShadowedObject();
    string evidence, limb_string;
    evidence = "";
    if(objectp(agent)) evidence += "You receive damage from "+agent->GetKeyName();
    else if(stringp(agent)) evidence += "You receive damage from "+agent;
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
    else limb_string = ", and I can't tell where I'm hit. ";
    if(limbs) { 
        evidence += ", body part(s) affected: ";
        evidence += limb_string + ".";
    }
    this_object()->eventForce("say "+evidence);
    hp = this_object()->GetHealthPoints();

    if(!agent) agent = this_object();
    if(!type) type = 0;
    if(!x) x = 0;
    if(!internal) internal = 0;
    if(!limbs) limbs = "";

    if(ob) ob->eventReceiveDamage(agent, type, x, internal, limbs);

    damage = this_object()->GetHealthPoints();
    damdiff = hp - damage;
    this_object()->eventForce("say actual damage done: "+damdiff);
}

int RemoveLimb(string limb, object agent){
    object ob = GetShadowedObject();
    if(!ob) return;
    if(ob->GetKeyName() == "dummy"){
        this_object()->eventForce("say My "+limb+" has received enough damage to sever it. "
                "However, since I am a training dummy, I'll be keeping it.");
        return 1;
    }
    return ob->RemoveLimb(limb, agent);
}
