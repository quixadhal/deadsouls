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
    AddHP(damdiff+1);
}

int RemoveLimb(string limb, object agent){
    eventForce("say My "+limb+" has received enough damage to sever it. "
      "However, since I am a training dummy, I'll be keeping it.");
    return 1;
}

void init(){
    ::init();
}
