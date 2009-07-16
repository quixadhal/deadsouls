/*    /lib/obj/storage.c
 *    From the Dead Souls Mud Library
 *    The standard object for storing things
 *    Created by Descartes of Borg 940212
 *    Version: @(#) storage.c 1.7@(#)
 *    Last modified: 96/12/31
 */

#include <lib.h>

inherit LIB_HOLDER;
inherit LIB_SEAL;

private int CanClose = 0;
private int CanLock  = 0;
private int MaxRecurseDepth  = 3;
private int RecurseDepth  = 1;
private static mixed my_save = ({});

int GetMaxRecurseDepth(){
    return MaxRecurseDepth;
}

int GetRecurseDepth(){
    return RecurseDepth;
}

int SetMaxRecurseDepth(int i){
    MaxRecurseDepth =i;
    return 1;
}

int SetRecurseDepth(int i){
    RecurseDepth=i;
    return 1;
}

int AddRecurseDepth(int i){
    RecurseDepth += i;
    return 1;
}

int GetCanClose(){
    return CanClose;
}

int SetCanClose(int x){
    CanClose = x;
}

int GetCanLock(){
    return CanLock;
}

int SetCanLock(int x){
    return SetCanClose(CanLock = x);
}

void SetKey(string key){ 
    SetKeys(key);
} 

int GetOpacity(){
    return holder::GetOpacity();
}

int SetOpacity(mixed arg){
    return holder::SetOpacity(arg);
}

int GetRadiantLight(int ambient){
    return holder::GetRadiantLight(ambient);
}

//string array GetSave(){
//    string *i_save, *s_save;
//    i_save = item::GetSave();
//    s_save = seal::GetSave();
//    my_save += i_save + s_save;
//    my_save += ({ "Closed","CanClose", "CanLock", "RecurseDepth" , "MaxRecurseDepth" });
//    my_save = distinct_array(my_save);
//    return my_save;
//}

//static mixed array AddSave(mixed array vars){
//    my_save = distinct_array(my_save + vars);
//    return item::AddSave(my_save);
//}

//int SetSaveRecurse(int x){
//    return item::SetSaveRecurse(x);
//}

mixed CanClose(object who, string id){
    if( !GetCanClose() ){
        return 0;
    }
    else {
        return seal::CanClose(who, id);
    }
}

mixed CanGetFrom(object who, object item){
    mixed tmp = holder::CanGetFrom(who, item);

    if( tmp != 1 ){
        return tmp;
    }
    if( GetClosed() ){
        return capitalize(GetDefiniteShort()) + " is closed.";
    }
    return 1;
}

mixed CanLock(object who, string id){
    if( !GetCanLock() ){
        return 0;
    }
    else {
        return seal::CanLock(who, id);
    }
}

mixed CanOpen(object who, string id){
    if( !GetCanClose() ){
        return 0;
    }
    else {
        return seal::CanOpen(who, id);
    }
}

mixed CanPick(object who, string id){
    if( !GetCanLock() ){
        return "It isn't lockable in the first place.";
    }
    else {
        return seal::CanPick(who, id);
    }
}

mixed CanPutInto(object who, object what){
    mixed tmp;
    int mydepth,yourdepth,total,indirectp,indirectpp;
    string wherefrom,stackstring;
    string *callstack;

    if(!tmp = holder::CanPutInto(who, what)){
        if(GetClosed()) return capitalize(GetDefiniteShort()) + " is closed right now.";
        else return "You can't do that right now.";
    }

    wherefrom=origin();
    callstack=call_stack(2);
    stackstring=implode(callstack," ");
    indirectp=member_array("indirect_put_obj_word_obj",callstack);
    indirectpp=member_array("indirect_put_objs_word_obj",callstack);


    if( tmp != 1 ){
        if( GetClosed() ) return capitalize(GetDefiniteShort()) + " is closed.";
        else return "You can't do that at this time.";
    }
    if( GetClosed() ){
        return capitalize(GetDefiniteShort()) + " is closed.";
    }

    if(inherits(LIB_STORAGE, what) ){
        yourdepth = what->GetRecurseDepth();
        mydepth = this_object()->GetRecurseDepth();
        if(yourdepth && mydepth) total = yourdepth + mydepth;
        if(total && total > this_object()->GetMaxRecurseDepth()) return "Doesn't fit.";
    }

    return 1;
}

varargs mixed CanShowInterior(object who, object target){
    if( GetClosed() && this_object()->GetOpacity() > 33){
        return capitalize(GetDefiniteShort()) + " is closed.";
    }
    else return holder::CanShowInterior();
}

mixed CanUnlock(object who, string id, object key){
    if( !GetCanLock() ){
        return 0;
    }
    else {
        return seal::CanUnlock(who, id, key);
    }
}

int eventReceiveObject(object ob){
    if( GetClosed() ){
        return 0;
    }

    return holder::eventReceiveObject(ob);
}

void PutCheck(){
    if(RecurseDepth >= MaxRecurseDepth){
        this_object()->SetPreventPut("You have enough containers "+
                "inside containers there. This one will have to stay out.");
    }
}

void create(){
    //string *i_save, *s_save;
    holder::create();
    //item::create();
    seal::create();
    //i_save = item::GetSave();
    //s_save = seal::GetSave();
    //my_save = i_save + s_save;
    //my_save += ({ "Closed","CanClose", "CanLock", "RecurseDepth" , "MaxRecurseDepth" });
    //my_save = distinct_array(my_save);
    //item::AddSave(my_save);
    PutCheck();
}

int inventory_accessible(){
    return seal::inventory_accessible();
}

int inventory_visible(){
    return (seal::inventory_visible() || holder::inventory_visible());
}
