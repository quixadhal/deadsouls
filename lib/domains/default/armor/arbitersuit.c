#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_BANE;
inherit LIB_WORN_STORAGE;
object arbitershadow;
int active = 1;
int charge = 20000;
int maxcharge = 20000;
int disguised = 0;

varargs mixed GetSuitHelp(mixed who, string where);

static void create(){
    ::create();
    SetKeyName("arbiter suit");
    SetId(({"suit", "armor"}));
    SetAdjectives(({"arbiter","suit of","powered","formidable", "formidable looking"}));
    SetShort("a suit of powered arbiter armor");
    SetLong("A formidable looking suit of powered armor used by "+
            "forces of the Extant Authority. This one appears to be one of "+
            "the arbiter variety.");
    SetMass(7000);
    SetMatching(0);
    SetBaseCost("silver",50000);
    SetArmorType(A_CUSTOM);
    SetRestrictLimbs( ({ 
                "torso", "head", "neck",
                "right arm", "left arm",
                "right leg", "left leg",
                "right hand", "left hand",
                "right foot", "left foot",
                }) );
    AddSave( ({ "charge", "disguised" }) );
    SetMaxCarry(5000);
    SetWear((: GetSuitHelp :));
}

void init(){
    ::init();
    add_action("GetSuitHelp", "help");
}

varargs mixed GetSuitHelp(mixed who, string where){
    string ret, str;
    object env;
    if(!who) return 0;
    if(stringp(who)){
        str = who;
        who = this_player();
    }
    else str = GetKeyName();
    env = environment(who);
    if(query_verb() == "wear" || (str && answers_to(str, this_object()))){
        if(environment() == who && charge){
            ret = "The suit's Heads Up Display crackles to life and reads:\n ";
            ret += "%^GREEN%^This suit allows you, mighty Arbiter, to travel in hazardous terrain "+
                "with a minimum of inconvenience. In the interest of fulfilling the exigencies "+
                "of your duties to the Extant Authority, this suit provides the following "+
                "improvements to your abilities:\n\n"+
                "* Good vision in all light conditions.\n"+
                "* A constant supply of breathable air.\n"+
                "* Substantial enhancement of strength, coordination, agility, and durability.\n"+
                "* Substantial enhancement of unarmed combat capability.\n"+
                "* Heads Up Display of key environmental information.\n"+
                "* Grid coordinate information where available.\n"+
                "* Protection from all forms of external damage.\n"+
                "* Immunity from disease.\n"+
                "\nNote that once the power level of the suit reaches zero, all enhancements "+
                "become unavailable.%^RESET%^";
            who->eventPrint("You wear "+GetShort()+".");
            if(env) tell_room(env, who->GetName()+" wears "+
                    GetShort()+".", ({who}));
            return 1;
        }
    }
    return 0;
}

mixed eventEquip(object who, string array limbs){
    mixed success = ::eventEquip(who, limbs);
    arbitershadow = new("/shadows/arbiter");
    if(arbitershadow) arbitershadow->SetDisguised(disguised);
    if(success){
        if(arbitershadow) arbitershadow->eventShadow(who);
    }
    else if(arbitershadow) destruct(arbitershadow);
    return success;
}

varargs mixed eventUnequip(object who) {
    mixed success;
    if(!who) who = this_player();
    success = ::eventUnequip(who);
    if(success){
        if(arbitershadow) who->unarbitershadow();
    }
    return success;
}

int GetActive(){
    if(!charge) return 0;
    return active;
}

int SetActive(int i){
    if(i) active = 1;
    else active = 0;
    return active;
}

int eventDecrementCharge(int i){
    int perc;
    object env = environment();
    if(!env) return 0;

    if(!GetWorn()) return 0;
    if(previous_object() != arbitershadow) return 0;
    if(charge < 1) charge = 0;
    else { 
        if(!i) charge--;
        else charge -= i;
        if(charge < 0) charge = 0;
    }

    perc = to_int(percent(charge, maxcharge));
    if(perc < 10){
        if(living(env) && creatorp(env)){
            env->eventPrint("Your creator powers magically recharge the "+
                    remove_article(GetShort())+".");
            charge = maxcharge;
            return charge;
        }
        tell_object(env,"The "+remove_article(GetShort())+" beeps loudly!");
        return charge;
    }

    if(perc < 20){
        if(living(env) && creatorp(env)){
            env->eventPrint("Your creator powers magically recharge the "+
                    remove_article(GetShort())+".");
            charge = maxcharge;
            return charge;
        }
        tell_object(env,"The "+remove_article(GetShort())+" beeps softly.");
        return charge;
    }

    return charge;
}

int GetRemainingCharge(){
    if(!charge || !GetWorn()){
        if(arbitershadow) arbitershadow->eventUnshadow();
        return 0;
    }
    return charge;
}

int GetMaxCharge(){
    return maxcharge;
}

int SetCharge(int i){
    charge = i;
    return charge;
}

int SetMaxCharge(int i){
    maxcharge = i;
    return maxcharge;
}

string array GetBane(){
    if(GetActive()) return ({ "all" });
    else return ({});
}

string array QueryBane(){
    if(GetActive()) return ({ "all" });
    else return ({});
}
