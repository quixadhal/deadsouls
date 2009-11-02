#include <lib.h>
#include "include/clip.h"
#include <vendor_types.h>
inherit LIB_STORAGE;
inherit LIB_LOAD;
private int caliber, millimeter, MaxAmmo, ammo;
private string firearmtype,ammotype;
void create(){
    ::create();
    SetKeyName("magazine");
    SetId(({"clip","magazine"}));
    SetAdjectives(({"ammunition","pistol"}));
    SetShort("a pistol ammunition clip");
    SetLong("This is a slender, spring-loaded container for semiautomatic "+
            "pistol ammunition.");
    SetMass(10);
    SetValue(1);
    SetVendorType(VT_TREASURE);
}
int CanReceive(object ob){
    string *namen;
    namen=ob->GetId();
    if(member_array("bullet",namen) == -1){
        write("Only bullets fit into the magazine.");
        return 0;
    }
    if(ob->GetAmmoType() != this_object()->GetAmmoType() ){
        write("That round is not the correct type for the magazine.");
        return 0;
    }
    if(ob->GetMillimeter() != this_object()->GetMillimeter() ){
        write("That round is not the correct size for the magazine.");
        return 0;
    }
    if(ob->GetCaliber() != this_object()->GetCaliber() ){
        write("That round is not the correct caliber for the magazine.");
        return 0;
    }
    if(ob->GetFirearmType() != "auto"){
        write("That round is not a semiautomatic round.");
        return 0;
    }
    if(sizeof(all_inventory()) >= MaxAmmo){
        write("The magazine is filled to capacity.");
        return 0;
    }
    ammo++;
    return 1;
}
int CanRelease(object ob){
    ammo--;
    return 1;
}
int MinusAmmo(int i){ ammo -= i; return 1; }
int PlusAmmo(int i){ ammo += i; return 1; }
int SetMaxAmmo(int i){ MaxAmmo=i; return 1; }
int SetFirearmType(string str){firearmtype=str; return 1; }
int SetAmmoType(string str){ammotype=str; return 1; }
int SetCaliber(int x){ caliber=x; return 1; }
int SetMillimeter(int x){ millimeter=x; return 1; }
int GetMillimeter(){ return millimeter; }
int GetCaliber(){ return caliber; }
string GetFirearmType(){ return firearmtype; }
string GetAmmoType(){ return ammotype; }

varargs mixed eventLoad(object who, object where){
    int success, err;
    mixed type = where->GetFirearmType();
    if(base_name(where) != LIB_FIREARM &&
            !inherits(LIB_FIREARM,where)){
        write("This magazine is for a firearm.");
        return 1;
    }
    if(!stringp(type) || type != "auto"){
        write("This ammunition magazine is for use with auto firearms.");
        return 1;
    }
    if(where->GetCaliber() != GetCaliber()){
        write("That is not the right caliber.");
        return 1;
    }
    if(where->GetMaxLoaded()){
        write("It's already got a magazine.");
        return 1;
    }
    err = catch(success = eventMove(where) );
    if(err || !success){
        write("There seems to be a problem loading it.");
        return 1;
    }
    else {
        write("You load your "+where->GetFirearmName()+".");
        say(this_player()->GetName()+" loads an ammunition "+
                "clip into "+possessive(this_player())+" "+    
                where->GetFirearmName()+".");
        where->SetLoaded(1);
    }
    return 1; 
}

varargs mixed eventUnload(mixed where){
    int success, err, inv;
    object env;
    mixed type = where->GetFirearmType();
    object prev = previous_object();
    env = environment();
    if((base_name(previous_object()) == LIB_ROUND ||
                inherits(LIB_ROUND, previous_object())) && intp(where)){
        if(!(inv = sizeof(all_inventory()))){
            write("It's already fully unloaded.");
            return 1;
        }
        if(inv < where) where = inv;
        err=catch(success=all_inventory()[0..inv-1]->eventMove(this_player()));
        if(err || !success){
            write("It seems you weren't able to unload all you wanted.");
            return 1;
        }
        else {
            write("You unload "+cardinal(where)+" "+
                    remove_article(previous_object()->GetShort())+" from the "+
                    remove_article(GetShort())+".");
            return 1;
        }
    }
    if(!env) return 0;
    if(!type) type = env->GetFirearmType();
    if(!stringp(type)){
        write("This ammunition magazine is not in a firearm.");
        return 1;
    }
    if(env != where){ 
        write("You seem confused about what to unload from where.");
        return 1;
    }
    env = environment(env);
    if(!env || env != this_player()){
        write("You aren't close enough to the firearm.");
        return 1;
    }
    err = catch(success = where->eventUnload(env) );
    if(err || !success){
        write("There seems to be a problem loading it.");
        return 1;
    }
    else {
        where->SetLoaded(0);
    }
    return 1;
}
