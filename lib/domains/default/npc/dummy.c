#include <lib.h>
#include <daemons.h>
#include <damage_types.h>
inherit LIB_NPC;

static void create(){
    npc::create();
    SetKeyName("dummy");
    SetId( ({"dummy","mokujin","buster"}) );
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
    SetRace("dummy");
    SetClass("fighter");
    SetGender("neuter");
    SetMaxHealthPoints(9000);
    SetHealthPoints(9000);
}

varargs int eventReceiveDamage(object agent, int type, int x, int internal, mixed limbs) {
    int hp, damage, damdiff;
    string evidence, limb_string;
    evidence = "";
    if(objectp(agent)) evidence += "I receive damage from "+agent->GetKeyName();
    else evidence += "I receive damage from "+agent;
    evidence += ".";
    if(type) {
        string *damtypes = TYPES_D->eventCalculateTypes("damage", type);
        if(type && sizeof(damtypes)) {
            string verboid;
            if(sizeof(damtypes) > 1) verboid = "s are ";
            else verboid = " is ";

            evidence += " Damage type"+verboid;
            evidence += implode(damtypes,", ");
        }
        else evidence += " Damage type is UNKNOWN";
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
    if(query_verb() == "eval" || query_verb() == "amputate" ||
            query_verb() == "call"){
        return ::RemoveLimb(limb, agent);
    }
    eventForce("say My "+limb+" has received enough damage to sever it. "
            "However, since I am a training dummy, I'll be keeping it.");
    return 1;
}

void init(){
    ::init();
}
