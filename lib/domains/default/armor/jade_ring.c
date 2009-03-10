#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_ARMOR;
object ringshadow;

string LongRet(mixed whom){
    string ret = "A green ring, glowing with unearthly power.";
    if(!whom || !objectp(whom) || !living(whom)) whom = this_player();
    if(creatorp(whom) ||
            member_group(whom,"TEST")){
        ret += "\nTo enable damage protection, type: protection on";
        ret += "\nTo enable damage reporting, type: reporting on";
        ret += "\nTo make a creature report its damage: enablereport <name>";
        ret += "\nTo make it stop reporting its damage: disablereport <name>\n";
    }
    return ret;
}

static void create(){
    armor::create();
    SetKeyName("jade ring");
    SetId(({"ring","ward","jade"}));
    SetAdjectives( ({"jade","green","power","powerful"}) );
    SetShort("a jade ring");
    SetLong( (: LongRet :) );
    SetMass(1);
    SetBaseCost("silver",5000);
    SetArmorType(A_RING);
}

void init(){
    armor::init();
    add_action("SetProtection", "protection");
    add_action("SetReporting", "reporting");
    add_action("SetNPCReporting", "enablereport");
    add_action("UnsetNPCReporting", "disablereport");
}

mixed CanEquip(object who, string array limbs){
    if(who && !creatorp(who) && !member_group(who,"TEST")){
        return "Somehow it just won't go on. Strange, isn't it?";
        return 0;
    }
    else return armor::CanEquip(who, limbs);
}

int eventRegenerate(object who){
    int i;
    string *stumps;
    stumps=who->GetMissingLimbs();
    if(sizeof(stumps)) {
        for(i=0;i<sizeof(stumps);i++){
            who->RestoreLimb(stumps[i]);
        }
    }
    return 1;
}

int PerformHeal(){
    object who;
    mapping stumps;
    string *limb_arr = ({});
    int i, mhp, msp, mmp, self;

    who = this_player();

    mhp = who->GetMaxHealthPoints();
    msp = who->GetMaxStaminaPoints();
    mmp = who->GetMaxMagicPoints();
    who->AddHP(mhp);
    who->AddStaminaPoints(msp);
    who->AddMagicPoints(mmp);
    if(who->GetPoison() > 0) who->AddPoison(0 - who->GetPoison());
    eventRegenerate(who);
    limb_arr = who->GetLimbs();
    foreach(string limb in limb_arr) {
        who->HealLimb(limb);
    }
    return 1;
}

int CheckRing(){
    if(environment() != this_player()) return 0;
    if(!(this_object()->GetWorn())) return 0;
    if(!creatorp(environment()) && 
            !member_group(environment(),"TEST")) return -1;
    return 1;
}

int SetNPCReporting(string name){
    object *sombras = ({});
    object ob;
    if(!CheckRing() || !ringshadow){
        write("The ring must be worn by you in order to access its power.");
        return 1;
    }
    if(CheckRing() == -1){
        write("The ring's power can only be harnessed by special people. That means, \"not you\".");
        return 1;
    }
    if(!(ob = present(name,environment(this_player())))){
        write("No such creature is here.");
        return 1;
    }
    sombras = keys(ob->GetShadows());
    if(sizeof(sombras)){
        foreach(object element in sombras){
            if(base_name(element) == "/shadows/diag"){
                write("That creature is already reporting its damage status.");
                return 1;
            }
        }
    }
    new("/shadows/diag")->eventShadow(ob);
    write("Damage reporting enabled for "+name+".");
    return 1;
}

int UnsetNPCReporting(string name){
    object *sombras = ({});
    object ob;
    int stat = 0;
    if(!CheckRing() || !ringshadow){
        write("The ring must be worn by you in order to access its power.");
        return 1;
    }
    if(CheckRing() == -1){
        write("The ring's power can only be harnessed by special people. That means, \"not you\".");
        return 1;
    }
    if(!(ob = present(name,environment(this_player())))){
        write("No such creature is here.");
        return 1;
    }
    sombras = keys(ob->GetShadows());
    if(sizeof(sombras)){
        foreach(mixed element in sombras){
            if(element && objectp(element) && base_name(element) == "/shadows/diag"){
                stat = 1;
                element->eventUnshadow();
            }
        }
    }
    if(!stat) write("That creature does not have reporting enabled.");
    else write("Damage reporting disabled for "+name+".");
    return 1;
}

int SetProtection(string str){
    int booly = 0;
    if(!CheckRing() || !ringshadow){
        write("The ring must be worn by you in order to access its power.");
        return 1;
    }
    if(CheckRing() == -1){
        write("The ring's power can only be harnessed by special people. That means, \"not you\".");
        return 1;
    } 
    if(str == "on") booly=1;
    write("You set the ring's protection to: "+(booly ? "on" : "off")+".");
    if(ringshadow) ringshadow->JadeProtection(booly);
    return 1;
}

int SetReporting(string str){
    int booly = 0;
    if(!CheckRing() || !ringshadow){
        write("The ring must be worn by you in order to access its power.");
        return 1;
    }
    if(CheckRing() == -1){
        write("The ring's power can only be harnessed by special people. That means, \"not you\".");
        return 1;
    }
    if(str == "on") booly=1;
    write("You set the ring's reporting to: "+(booly ? "on" : "off")+".");
    if(ringshadow) ringshadow->JadeReporting(booly);
    return 1;
}

mixed eventEquip(object who, string array limbs){
    int ok = 0;
    mixed success = armor::eventEquip(who, limbs);
    if(!ringshadow){
        object *sombras = keys(who->GetShadows());
        ok = 1;
        ringshadow = new("/shadows/ring");
        foreach(object element in sombras){
            if(base_name(element) == base_name(ringshadow)){
                ok = 0;
                if(ringshadow) destruct(ringshadow);
                ringshadow = element;
            }
        }
    }
    if(success){
        PerformHeal();
        if(ok){
            if(ringshadow) ringshadow->eventShadow(who);
            if(ringshadow) ringshadow->JadeProtection(1);
        }
    }
    else if(ringshadow) destruct(ringshadow);
    return success;
}

varargs mixed eventUnequip(object who) {
    mixed success;
    if(!who) who = this_player();
    success = armor::eventUnequip(who);
    if(success){
        if(ringshadow) ringshadow->eventUnshadow(ringshadow);
    }
    return success;
}
