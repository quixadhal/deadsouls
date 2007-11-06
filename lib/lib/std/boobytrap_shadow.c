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

void create(){
    parse_init();
}

int eventShadow(object ob){
    return shadow::eventShadow(ob);
}

varargs mixed SpringTrap(mixed arg1, mixed arg2){
    write("You set off a trap!");
    say(this_player()->GetCapName()+" sets off a trap!");
    call_out( (: eventUnshadow :), 1);
    return 1;
}

varargs mixed eventDisarm(mixed args...){
    write("You successfully disarm the trap!");
    eventUnshadow();
    return 1;
}

varargs mixed CanOpen(object who, object tool) {
    object ob = GetShadowedObject();
    if(ob->GetDoor()) ob = ob->GetDoor();
    return ob->CanOpen(who, tool);
}

varargs mixed eventOpen(object who, object tool) {
    if(traptype & BOOBYTRAP_OPEN) SpringTrap(who, tool);
    return GetShadowedObject()->eventOpen(who, tool);
}

varargs mixed CanClose(object who, string id){
    object ob = GetShadowedObject();
    if(ob->GetDoor()) ob = ob->GetDoor();
    return ob->CanClose(who, id);
}

mixed eventClose(object who){
    if(traptype & BOOBYTRAP_CLOSE) SpringTrap(who);
    return GetShadowedObject()->eventClose(who);
}

mixed direct_get_obj(object target){
    return GetShadowedObject()->direct_get_obj(target);
}

mixed direct_get_obj_out_of_obj(object target, object src){
    return GetShadowedObject()->direct_get_obj_out_of_obj(target, src);
}

mixed direct_get_obj_from_obj(object target, object src) {
    return  GetShadowedObject()->direct_get_obj_out_of_obj(target, src);
}

mixed direct_get_obj_obj(object target, object src) {
    return  GetShadowedObject()->direct_get_obj_out_of_obj(target, src);
}

mixed GetPreventGet() {
    return GetShadowedObject()->GetPreventGet();
}

mixed CanGet(object who){
    return GetShadowedObject()->CanGet(who);
}

mixed eventGet(object who){
    if(traptype & BOOBYTRAP_GET) SpringTrap(who);
    return GetShadowedObject()->eventGet(who);
}

mixed direct_drop_obj(object target){
    return GetShadowedObject()->direct_drop_obj(target);
}

int GetDestructOnDrop() {
    return GetShadowedObject()->GetDestructOnDrop();
}

mixed GetPreventDrop() {
    return GetShadowedObject()->GetPreventDrop();
}

mixed CanDrop(object who){
    return GetShadowedObject()->CanDrop(who);
}

mixed eventDrop(object who){
    if(traptype & BOOBYTRAP_DROP) SpringTrap(who);
    return GetShadowedObject()->eventDrop(who);
}

mixed CanLock(object who, string id){
    return GetShadowedObject()->CanLock(who, id);
}

varargs mixed eventLock(object who, mixed arg1, mixed arg2){
    if(traptype & BOOBYTRAP_LOCK) SpringTrap(who);
    return GetShadowedObject()->eventLock(who,arg1,arg2);
}

varargs mixed CanUnlock(object who, string id, object key){
    return GetShadowedObject()->CanUnlock(who, id, key);
}

varargs mixed eventUnlock(object who, mixed arg1, mixed arg2){
    if(traptype & BOOBYTRAP_UNLOCK) SpringTrap(who);
    return GetShadowedObject()->eventUnlock(who, arg1, arg2);
}

mixed CanSteal(object who){
    return GetShadowedObject()->CanSteal(who);
}

mixed eventSteal(object who){
    if(traptype & BOOBYTRAP_STEAL) SpringTrap(who);
    return GetShadowedObject()->eventSteal(who);
}

mixed CanPick(object who, string id){
    return GetShadowedObject()->CanPick(who, id);
}

varargs mixed eventPick(object who, string str, object tool){
    if(traptype & BOOBYTRAP_PICK) SpringTrap(who);
    return GetShadowedObject()->eventPick(who, str, tool);
}

string GetShort(){
    return GetShadowedObject()->GetShort();
}

string GetKeyName(){
    return GetShadowedObject()->GetKeyName();
}
