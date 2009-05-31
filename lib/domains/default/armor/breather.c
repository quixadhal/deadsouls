#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_ARMOR;
object breathershadow;

int charge = 1000;
int maxcharge = 1000;

string LongD(){
    string ret = "This remarkably small device fits over the "+
        "wearer's mouth, and provides a long supply of oxygenated "+
        "air. Its current charge level is "+
        to_int(percent(charge,maxcharge))+" percent.";
    return ret;
}

static void create(){
    armor::create();
    SetKeyName("breathing mask");
    SetId(({"mask","breather","a99","apparatus","device"}));
    SetAdjectives(({"a99","breathing"}));
    SetShort("an A99 breathing device");
    SetLong( (: LongD :) );
    SetMass(50);
    SetBaseCost("silver",1000);
    SetArmorType(A_CUSTOM);
    SetRestrictLimbs( ({ "head"}) );
    AddSave( ({ "charge" }) );
}

void init(){
    ::init();
}

int SetCharge(int i){
    charge = i;
}

int SetMaxCharge(int i){
    maxcharge = i;
}

mixed eventEquip(object who, string array limbs){
    mixed success = armor::eventEquip(who, limbs);
    breathershadow = new("/shadows/breather");
    if(success){
        if(breathershadow) breathershadow->eventShadow(who);
    }
    else if(breathershadow) destruct(breathershadow);
    return success;
}

varargs mixed eventUnequip(object who) {
    mixed success;
    if(!who) who = this_player();
    success = armor::eventUnequip(who);
    if(success){
        if(breathershadow) breathershadow->eventUnshadow();
    }
    if(breathershadow) destruct(breathershadow);
    return success;
}

int eventDecrementCharge(){
    int perc;
    string name;
    object room, env = environment(this_object());
    if(!env || !GetWorn()) return 0;
    if(previous_object() != breathershadow) return 0;
    if(charge < 1) charge = 0;
    else charge--;
    if(living(env)) room = environment(env);

    perc = to_int(percent(charge, maxcharge));
    if(perc < 10){
        tell_object(env,"The "+remove_article(GetShort())+" beeps loudly!");
        if(room){
            name = env->GetName();
            tell_room(room, name+"'s breathing device beeps.", ({ env }));
        }
        return charge;
    }

    if(perc < 20){
        tell_object(env,"The "+remove_article(GetShort())+" beeps softly.");
        return charge;
    }

    return charge;
}

int GetRemainingCharge(){
    if(!charge){
        if(breathershadow) breathershadow->eventUnshadow();
    }
    if(!GetWorn()) return 0;
    return charge;
}

