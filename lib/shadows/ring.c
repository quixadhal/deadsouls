#include <daemons.h>
#include <damage_types.h>
inherit LIB_SHADOW;
int reporting = 1;
int protecting = 1;

varargs int eventReceiveDamage(mixed agent, int type, int x, int internal, mixed limbs) {
    int stamina, fatigue, hp, damage, damdiff;
    object ob = GetShadowedObject();
    string evidence, limb_string;
    if(reporting){
        string *damtypes = TYPES_D->eventCalculateTypes("damage", type);
        evidence = "%^BOLD%^%^RED%^";

        if(objectp(agent)) evidence += "You receive damage from "+agent->GetKeyName();
        else if(stringp(agent)) evidence += "You receive damage from "+agent;
        evidence +=".";

        if(type && sizeof(damtypes)) {
            string verboid;
            if(sizeof(damtypes) > 1) verboid = "s are ";
            else verboid = " is ";

            evidence += " Damage type"+verboid;
            evidence += lower_case(implode(damtypes,", "));
        }
        else evidence += " Damage type is UNKNOWN";

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

        if(x > this_object()->GetHealthPoints() &&
                !(this_object()->GetGodMode()) ){
            this_object()->eventPrint("%^RED%^This could have been a fatal hit. "+
                    "The ring's safety protocol prevents \"actual damage\" calculation "+
                    "from proceeding.%^RESET%^");
            return 1;
        }

        if(ob) ob->eventReceiveDamage(agent, type, x, internal, limbs);

        damage = this_object()->GetHealthPoints();
        fatigue = stamina - this_object()->GetStaminaPoints();
        damdiff = hp - damage;
        this_object()->eventPrint("%^RED%^Actual damage done: "+damdiff+"%^RESET%^");
        this_object()->eventPrint("%^YELLOW%^Stamina sapped: "+fatigue+"%^RESET%^");
        if(protecting && ob){
            ob->AddHP(damdiff+1);
            this_object()->AddStaminaPoints(fatigue+1);
        }
    }
    else if(!protecting && ob) 
        return ob->eventReceiveDamage(agent, type, x, internal, limbs);
    return 1;
}

int RemoveLimb(string limb, object agent){
    object ob = GetShadowedObject();
    if(!ob) return 0;
    if(protecting){
        this_object()->eventPrint("you have received enough damage to sever it. "+
                "However, The ring's protection prevents that.");
        return 1;
    }
    else return ob->RemoveLimb(limb, agent);
}

int AddHP(int hp){
    object ob = GetShadowedObject();
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
    if(ob) return ob->AddHP(hp);
    else return 1;
}

int JadeProtection(int i){
    object ob = GetShadowedObject();
    if(!this_player()) return 0;
    if(!ob || !(this_player() == ob )) return 0;
    protecting = i;
    return protecting;
}

int JadeReporting(int i){
    object ob = GetShadowedObject();
    if(!this_player()) return 0;
    if(!ob || !(this_player() == ob )) return 0;
    reporting = i;
    return reporting;
}

int eventUnshadow(){
    object ob = GetShadowedObject();
    if(!this_player()) return 0;
    if(!ob || !(this_player() == ob )) return 0;
    return ::eventUnshadow();
}
