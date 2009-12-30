#include <lib.h>
#include <daemons.h>
#include <dirs.h>
#include <vision.h>
#include <medium.h>
#include <terrain_types.h>
#include <damage_types.h>
#include <respiration_types.h>

inherit LIB_SHADOW;
inherit LIB_ID;

object me = this_object();
object suit;
int disguised, reporting = 1;

int SetDisguised(int i){
    if(i) disguised = 1;
    else disguised = 0;
    return disguised;
}

int GetDisguised(){
    return disguised;
}

int CheckDisguised(){
    object ob;
    if(clonep()) ob = GetShadowedObject();
    if(!ob || !disguised) return 0;
    else return 1;
}

int CheckSuit(){
    if(!suit){
        this_object()->unscoutshadow();
        return 0;
    }
    return 1;
}

varargs string GetExternalDesc(object who){
    object ob = GetShadowedObject();
    if(!clonep()) return "";
    if(CheckDisguised()) return "A Poleepkwa scout.";
    else return ob->GetExternalDesc(who);
}

varargs string GetLong(object who){
    object ob = GetShadowedObject();
    if(!clonep()) return "";
    if(CheckDisguised()) return "A Poleepkwa scout.";
    else return ob->GetLong(who);
}

string GetName(){
    object ob = GetShadowedObject();
    if(!clonep()) return "";
    if(CheckDisguised()) return "Scout";
    else return ob->GetName();
}

string GetCapName(){
    object ob = GetShadowedObject();
    if(!clonep()) return "";
    if(CheckDisguised()) return "Scout";
    else return ob->GetCapName();
}

string GetKeyName(){
    object ob = GetShadowedObject();
    if(!clonep()) return "";
    if(CheckDisguised()) return "scout";
    else return ob->GetKeyName();
}

string GetShort(){
    object ob = GetShadowedObject();
    if(!clonep()) return "";
    if(CheckDisguised()) return "a scout";
    else return ob->GetShort();
}

string GetRace(){
    object ob = GetShadowedObject();
    if(!clonep()) return "";
    if(CheckDisguised()) return "poleepkwa";
    else return ob->GetRace();
}

int eventShadow(object whom){
    object *shadows = keys(whom->GetShadows());
    if(sizeof(shadows)){
        foreach(object shade in shadows){
            if(base_name(shade) == base_name(me)) return 0;
        }
    }
    if(base_name(previous_object()) == "/domains/town/armor/pscoutsuit"
            || base_name(previous_object()) == "/domains/default/armor/pscoutsuit"){
        ::eventShadow(whom);
        suit = previous_object();
        if(CheckDisguised()) whom->SetId(({"scout"}));
        if(CheckDisguised()) whom->cmdParseRefresh();
        return 1;
    }
    return 0;
}

int unscoutshadow(){
    object ob = GetShadowedObject();
    if(!ob) return 0;
    ob->SetId();
    ob->cmdParseRefresh();
    eventUnshadow();
}

string array parse_command_id_list() {
    string array ids = (this_object()->GetId() + 
            (CheckDisguised() ? ({ "scout" }) : ({}) ) || ({}));
    return filter(ids, (: stringp($1) && ($1 != "") :));

}

string array parse_command_plural_id_list() {
    string array ids = (this_object()->GetId() +
            (CheckDisguised() ? ({ "scout" }) : ({}) ) || ({}));
    ids = filter(ids, (: stringp($1) && ($1 != "") :));
    return map(ids, (: pluralize :));
}

int GetCanBite(){
    return 0;
}

float AddStaminaPoints(mixed x) {
    float y;
    object ob = GetShadowedObject();
    if(!clonep()) return 0;
    if(!CheckSuit()) return 0;
    if(!ob) return 0;
    if(!suit->GetActive()) return ob->AddStaminaPoints(x);
    if( !intp(x) && !floatp(x) )
        error("Bad argument 1 to AddStaminaPoints().\n");
    x = abs(x);
    suit->eventDecrementCharge(x);
    return suit->GetRemainingCharge();
}

int RemoveLimb(string limb, mixed agent){
    object ob = GetShadowedObject();
    if(!CheckSuit()) return 0;
    if(!ob) return 0;
    if(suit->GetActive()){
        suit->eventDecrementCharge();
        return 0;
    }
    return ob->RemoveLimb(limb, agent);
}

varargs mixed GetEffectiveVision(mixed location, int raw_score){
    object ob = GetShadowedObject();
    if(!CheckSuit()) return 0;
    if(!ob) return 0;
    if(suit->GetActive()){
        suit->eventDecrementCharge();
        return VISION_CLEAR;
    }
    return ob->GetEffectiveVision(location, raw_score);
}

string GetResistance(int type){
    object ob = GetShadowedObject();
    string orig = ob->GetResistance(type);
    if(!CheckSuit()) return 0;
    if(!ob) return 0;
    if(suit->GetActive()){
        suit->eventDecrementCharge();
        if(type == GAS) return "immune";
    }
    return orig;
}

varargs int CanBreathe(mixed args...){
    object ob = GetShadowedObject();
    object env = environment(ob);
    int rtype, good;
    if(!ob || !env) return 0;
    good = ob->CanBreathe(args);
    if(!CheckSuit()) return good;
    rtype = ob->GetRespiration();
    if(suit->GetActive()){
        if(rtype & R_AIR){
            if(!good){
                suit->eventDecrementCharge();
                return 1;
            }
        }
        if(rtype & R_WATER || rtype & R_METHANE) return 0;
    }
    return ob->CanBreathe(args);
}

varargs int eventReceiveDamage(mixed agent, int type, int x, int internal, mixed limbs) {
    object ob = GetShadowedObject();
    string evidence, limb_string;

    if(!CheckSuit()) return 0;
    if(!ob) return 0;

    if(internal || !suit->GetActive() || suit->GetRemainingCharge() < 20){
        return ob->eventReceiveDamage(agent, type, x, internal, limbs);
    }

    if(reporting){
        string *damtypes = TYPES_D->eventCalculateTypes("damage", type);
        evidence = "The powered suit's Heads-Up-Display issues an alert:\n";
        evidence += "%^BOLD%^%^RED%^";
        if(objectp(agent)) evidence += "Damage received!";
        if(type && sizeof(damtypes)) {
            string verboid;
            if(sizeof(damtypes) > 1) verboid = "s are ";
            else verboid = " is ";

            evidence += " Damage type"+verboid;
            evidence += lower_case(implode(damtypes,", "));
        }

        else evidence += " Damage type is UNKNOWN";
    }

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
    else limb_string = ". Location indeterminate. ";
    if(limbs) { 
        evidence += ". Location: ";
        evidence += limb_string + ".";
    }
    this_object()->eventPrint(evidence+"%^RESET%^");

    this_object()->eventPrint("%^YELLOW%^Juice drained: "+x+" units.%^RESET%^");
    suit->eventDecrementCharge(x);
    return 1;
}

int AddLead(string ammo,int number){
    return 0;
}

void eventDescribeEnvironment(int verbose) {
    object ob = GetShadowedObject();
    object env;
    string extra = "%^CYAN%^Suit info:%^RESET%^\n";
    extra += "Juice left: "+to_int(percent(suit->GetRemainingCharge(),
                suit->GetMaxCharge()))+"%\n";
    if(!ob) return;
    if(!CheckSuit()){
        ob->eventDescribeEnvironment(verbose);
        return;
    }
    //extra += "Mystery number dam: "+suit->GetDamagePoints()+"\n";
    //extra += "Mystery number dets: "+suit->GetDeterioration()+"\n";
    //extra += "Mystery number broken: "+suit->GetBroken()+"\n";
    //extra += "Mystery number remaining: "+suit->GetRemainingCharge()+"\n";
    //extra += "Mystery number max: "+suit->GetMaxCharge()+"\n";
    //extra += "Mystery number active: "+suit->GetActive()+"\n";
    ob->eventDescribeEnvironment(verbose);
    message("system", extra, ob);
}

