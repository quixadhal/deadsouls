#include <lib.h>
#include <boobytraps.h>
inherit LIB_SHADOW;
inherit LIB_CLOSE;
inherit LIB_GET;
inherit LIB_DROP;
inherit LIB_LOCK;
inherit LIB_STEAL;

int traptype = 0;
int traplevel = 0;
int autoresets = 0;
string shadow_object = "";
string trap_description = "";

int SetTrapType(int i){
    traptype = i;
    return traptype;
}

int GetTrapType(){
    return traptype;
}

int SetTrapLevel(int i){
    traplevel = i;
    return traplevel;
}

int GetTrapLevel(){
    return traplevel;
}

string SetTrapDescription(string str){
    trap_description = str;
    return trap_description;
}

string GetTrapDescription(){
    return trap_description;
}

int SetAutoResets(int i){
    autoresets = i;
    return autoresets;
}

int GetAutoResets(){
    return autoresets;
}

void create(){
    parse_init();
}

int eventShadow(object ob){
    return shadow::eventShadow(ob);
}

varargs mixed SpringTrap(mixed arg1, mixed arg2){
    if(!autoresets){
        eventUnshadow();
    }
    else autoresets--;
    return 1;
}

varargs mixed eventDisarm(mixed args...){
    write("You successfully disarm the trap!");
    eventUnshadow();
    return 1;
}

varargs mixed CanOpen(object who, object tool){
    object ob = GetShadowedObject();
    if(!ob) return 0;
    if(ob->GetDoor()) ob = ob->GetDoor();
    return ob->CanOpen(who, tool);
}

varargs mixed eventOpen(object who, object tool){
    object ob = GetShadowedObject();
    mixed ret;
    if(!ob) return 0;
    ret = ob->eventOpen(who, tool);
    if(traptype & BOOBYTRAP_OPEN) SpringTrap(who, tool);
    return ret;
}

varargs mixed CanClose(object who, string id){
    object ob = GetShadowedObject();
    if(!ob) return 0;
    if(ob->GetDoor()) ob = ob->GetDoor();
    return ob->CanClose(who, id);
}

mixed eventClose(object who){
    object ob = GetShadowedObject();
    mixed ret;
    if(!ob) return 0;
    ret = ob->eventClose(who);
    if(traptype & BOOBYTRAP_CLOSE) SpringTrap(who);
    return ret;
}

mixed direct_get_obj(object target){
    object ob = GetShadowedObject();
    if(!ob) return 0;
    return ob->direct_get_obj(target);
}

mixed direct_get_obj_out_of_obj(object target, object src){
    object ob = GetShadowedObject();
    if(!ob) return 0;
    return ob->direct_get_obj_out_of_obj(target, src);
}

mixed direct_get_obj_from_obj(object target, object src){
    object ob = GetShadowedObject();
    if(!ob) return 0;
    return  ob->direct_get_obj_out_of_obj(target, src);
}

mixed direct_get_obj_obj(object target, object src){
    object ob = GetShadowedObject();
    if(!ob) return 0;
    return  ob->direct_get_obj_out_of_obj(target, src);
}

mixed GetPreventGet(){
    object ob = GetShadowedObject();
    if(!ob) return 0;
    return ob->GetPreventGet();
}

mixed CanGet(object who){
    object ob = GetShadowedObject();
    if(!ob) return 0;
    return ob->CanGet(who);
}

mixed eventGet(object who){
    object ob = GetShadowedObject();
    mixed ret;
    if(!ob) return 0;
    ret = ob->eventGet(who);
    if(traptype & BOOBYTRAP_GET) SpringTrap(who);
    return ret;
}

mixed direct_drop_obj(object target){
    object ob = GetShadowedObject();
    if(!ob) return 0;
    return ob->direct_drop_obj(target);
}

int GetDestructOnDrop(){
    object ob = GetShadowedObject();
    if(!ob) return 0;
    return ob->GetDestructOnDrop();
}

mixed GetPreventDrop(){
    object ob = GetShadowedObject();
    if(!ob) return 0;
    return ob->GetPreventDrop();
}

mixed CanDrop(object who){
    object ob = GetShadowedObject();
    if(!ob) return 0;
    return ob->CanDrop(who);
}

mixed eventDrop(object who){
    object ob = GetShadowedObject();
    mixed ret;
    if(!ob) return 0;
    ret = ob->eventDrop(who);
    if(traptype & BOOBYTRAP_DROP) SpringTrap(who);
    return ret;
}

mixed CanLock(object who, string id){
    object ob = GetShadowedObject();
    if(!ob) return 0;
    return ob->CanLock(who, id);
}

varargs mixed eventLock(object who, mixed arg1, mixed arg2){
    object ob = GetShadowedObject();
    mixed ret;
    if(!ob) return 0;
    ret = ob->eventLock(who,arg1,arg2);
    if(traptype & BOOBYTRAP_LOCK) SpringTrap(who);
    return ret;
}

varargs mixed CanUnlock(object who, string id, object key){
    object ob = GetShadowedObject();
    if(!ob) return 0;
    return ob->CanUnlock(who, id, key);
}

varargs mixed eventUnlock(object who, mixed arg1, mixed arg2){
    object ob = GetShadowedObject();
    mixed ret;
    if(!ob) return 0;
    ret = ob->eventUnlock(who, arg1, arg2);
    if(traptype & BOOBYTRAP_UNLOCK) SpringTrap(who);
    return ret;
}

mixed CanSteal(object who){
    object ob = GetShadowedObject();
    if(!ob) return 0;
    return ob->CanSteal(who);
}

mixed eventSteal(object who){
    object ob = GetShadowedObject();
    mixed ret;
    if(!ob) return 0;
    ret = ob->eventSteal(who);
    if(traptype & BOOBYTRAP_STEAL) SpringTrap(who);
    return ret;
}

mixed CanPick(object who, string id){
    object ob = GetShadowedObject();
    if(!ob) return 0;
    return ob->CanPick(who, id);
}

varargs mixed eventPick(object who, string str, object tool){
    object ob = GetShadowedObject();
    mixed ret;
    if(!ob) return 0;
    ret = ob->eventPick(who, str, tool);
    if(traptype & BOOBYTRAP_PICK) SpringTrap(who);
    return ret;
}

mixed direct_pick_str_on_obj(string str, object target, string str2,
        string id){
    object ob = GetShadowedObject();
    if(!ob) return 0;
    return ob->direct_pick_str_on_obj(str,target,str2,id);
}

mixed direct_pick_str_on_obj_with_obj(string str, object target, object tool,
        string str2, string targ_id){
    object ob = GetShadowedObject();
    if(!ob) return 0;
    return ob->direct_pick_str_on_obj_with_obj(str,target,tool,str2,targ_id);
}

string GetShort(){
    object ob = GetShadowedObject();
    if(!ob) return "";
    return ob->GetShort();
}

string GetKeyName(){
    object ob = GetShadowedObject();
    if(!ob) return "";
    return ob->GetKeyName();
}

mixed eventEquip(object who, string array limbs){
    object ob = GetShadowedObject();
    mixed ret;
    if(!ob) return 0;
    ret = ob->eventEquip(who, limbs);
    if(function_exists("GetWear",ob)){
        if(traptype & BOOBYTRAP_WEAR) SpringTrap(who);
    }
    else if(traptype & BOOBYTRAP_WIELD) SpringTrap(who);
    return ret;
}

mixed eventUnequip(object who){
    object ob = GetShadowedObject();
    mixed ret;
    if(!ob) return 0;
    ret = ob->eventUnequip(who);
    if(function_exists("GetWear",ob)){
        if(traptype & BOOBYTRAP_REMOVE) SpringTrap(who);
    }
    else if(traptype & BOOBYTRAP_UNWIELD) SpringTrap(who);
    return ret;
}
