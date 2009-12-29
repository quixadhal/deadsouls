#include <lib.h>

inherit LIB_BASE_STORAGE;
inherit LIB_BASE_ARMOR;
inherit LIB_SHADOW_HOOK;
inherit LIB_MONEY;

private static mixed *my_save = ({});

void create(){
    mixed a_save, s_save;
    base_armor::create();
    SetSaveRecurse(1);
    a_save = base_armor::GetSave();
    s_save = base_storage::GetSave();
    my_save = distinct_array(a_save + s_save);
    base_armor::AddSave(my_save);
    base_storage::create();
}

void init(){
    base_armor::init();
}

mixed direct_remove_obj(){
    return base_armor::direct_remove_obj();
}

mixed direct_wear_obj(){
    return base_armor::direct_wear_obj();
}

mixed eventEquip(object who, string array limbs){
    return base_armor::eventEquip(who, limbs);
}

varargs mixed eventUnequip(object who){
    return base_armor::eventUnequip(who);
}

varargs string GetEquippedDescription(object who){
    return base_armor::GetEquippedDescription(who);
}

varargs string GetExternalDesc(object who){
    return base_armor::GetExternalDesc(who);
}

string GetEquippedShort(){
    return base_armor::GetEquippedShort();
}

string array GetSave(){
    return base_armor::GetSave();
}

varargs string array AddSave(string *args){
    my_save += args;
    return base_armor::AddSave(my_save);
}
