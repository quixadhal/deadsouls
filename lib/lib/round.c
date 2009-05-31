#include <lib.h>
#include "include/round.h"
#include <vendor_types.h>
inherit LIB_ITEM;
inherit LIB_LOAD;
private int caliber,millimeter;
private string firearmtype,ammotype;
private string rifletype;
void create(){
    ::create();
    SetKeyName("round");
    SetId(({"round","bullet"}));
    SetShort("a generic pistol round");
    SetLong("This is a generic pistol round.");
    SetMass(1);
    SetValue(1);
    SetVendorType(VT_TREASURE);
}
int SetRifleType(string str){rifletype=str; return 1; }
int SetFirearmType(string str){firearmtype=str; return 1; }
int SetAmmoType(string str){ammotype=str; return 1; }
int SetCaliber(int x){ caliber=x; return 1; }
int SetMillimeter(int x){ millimeter=x; return 1; }
int GetMillimeter(){ return millimeter; }
int GetCaliber(){ return caliber; }
string GetRifleType(){ return rifletype; }
string GetFirearmType(){ return firearmtype; }
string GetAmmoType(){ return ammotype; }

varargs mixed eventLoad(object who, object where){
    int success, err;
    object cylinder;
    mixed type;
    if(where) type = where->GetFirearmType();
    else {
        write("Load it where?");
        return 0;
    }
    if(!stringp(type) && !stringp(type = where->GetAmmoType())){
        write("That seems the wrong thing to try to load with this.");
        return 1;
    }
    if(type == "acp"|| type == "nato") type = "auto";
    if(type == "revolver"){
        mixed tmparr = filter(all_inventory(where),
                (: base_name($1) == LIB_CYLINDER || inherits(LIB_CYLINDER, $1) :) );
        if(sizeof(tmparr)) cylinder = tmparr[0];
    }
    if(GetFirearmType() == "revolver" && ((base_name(where) != LIB_FIREARM &&
                    !inherits(LIB_FIREARM,where)) || type != "revolver")){
        write("This ammunition is for a revolver.");
        return 1;
    }
    if(GetFirearmType() == "auto" && (base_name(where) == LIB_FIREARM ||
                inherits(LIB_FIREARM,where))){
        write("That type of weapon is fed with an ammunition magazine.");
        return;
    }
    if(where->GetCaliber() != GetCaliber()){
        write("That is not the right caliber.");
        return 1;
    }
    if(where->GetMaxLoaded()){
        write("It's already fully loaded.");
        return 1;
    }
    err = catch(success = eventMove((cylinder || where)) );
    if(err || !success){
        write("There seems to be a problem loading it.");
        return 1;
    }
    else {
        if(cylinder) where->eventLoad(this_object());
        write("You load your "+where->GetShort()+".");
        say(this_player()->GetName()+" loads "+GetShort()+
                " into "+possessive(this_player())+" "+    
                remove_article(where->GetShort())+".");
    }
    return 1; 
}

varargs mixed eventUnload(object where){
    int success, err;
    object env;
    mixed type = where->GetFirearmType();
    env = environment();
    if(!env) return 0;
    if(base_name(env) == LIB_CYLINDER || inherits(LIB_CYLINDER, env)){
        env = environment(env);
    }
    if(!env) return 0;
    if(!type) type = env->GetFirearmType();
    if(!stringp(type)){
        write("The "+remove_article(GetShort())+" is not loaded anywhere.");
        return 1;
    }
    if(env != where){ 
        write("You seem confused about what to unload from where.");
        return 1;
    }
    env = environment(env);
    if(!env || env != this_player()){
        write("You aren't close enough to the "+
                remove_article(where->GetShort())+".");
        return 1;
    }
    err = catch(success = where->eventUnload(1) );
    if(err || !success){
        write("There seems to be a problem loading it.");
        return 1;
    }
    else {
        where->SetLoaded(0);
    }
    return 1;
}
