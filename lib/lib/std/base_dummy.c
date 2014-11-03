/*    /lib/obj/dummy.c
 *    From the Dead Souls Mud Library
 *    A dummy item object that handles item descriptions and such
 *    Created by Descartes of Borg 961014
 *    Version: @(#) dummy.c 1.18@(#)
 *    Last modified: 96/12/31
 */

#include <lib.h>
#include <function.h>

inherit LIB_CLEAN;
inherit LIB_DESCRIPTION;
inherit LIB_ENTER;
inherit LIB_ID;
inherit LIB_LISTEN;
inherit LIB_LOOK;
inherit LIB_RADIANCE;
inherit LIB_READ;
inherit LIB_SMELL;
inherit LIB_TOUCH;
inherit LIB_KNOCK;
inherit LIB_SCRATCH;
inherit LIB_SINK;

/* ***************** dummy.c attributes ***************** */

int isDummy(){
    return 1;
}
varargs string array SetId(mixed ids...){
    ids = id::SetId(ids);
    if( sizeof(ids) && !GetKeyName() ){
        SetKeyName(ids[0]);
        if( !GetShort() ){
            SetShort(add_article(ids[0]));
        }
    }
    return ids;
}
//varargs int GetInvis(object ob){
//    return 1;
//}

/* ********************* dummy.c events ****************** */
static int Destruct(){
    object env;
    int x;

    if( env = environment() ){
        env->eventReleaseObject(this_object());
        x = clean::Destruct();
        if( !x ){
            env->eventReceiveObject(this_object());
        }
        return x;
    }
    else return clean::Destruct();
}
mixed eventMove(mixed dest){
    object ob;
    string str;

    if(!valid_event(previous_object(), this_object())) return 0;

    str = "I am "+file_name(this_object())+", named "+identify(GetId())+", ";
    str += "and I have been asked to move ";
    str += "by "+identify(previous_object());

    if(previous_object(-1)) str += " at the request of "+identify(previous_object(-1));
    str += ". ";
    if(environment()) str += "\nMy current environment is "+file_name(environment())+".";
    else str += "\nI currently have no environment.";

    if( stringp(dest) ){
        ob = load_object(dest);
    }
    else {
        ob = dest;
    }
    if(ob){
        str += "\nMy intended destination is "+file_name(ob)+". ";
        move_object(ob);
        if( environment() != ob ){
            str += "\nThe move was not successful";
            return 0;
        }
        else {
            str += "\nThe move was successful.";
            eventAnnounceCanonicalId(environment());
            return 1;
        }
    }
    return 0; 
}

/* ******************* dummy.c driver applies ******************** */
varargs static void create(string array id, mixed long, string array adj){
    string str;
    str = "I am "+file_name(this_object())+" and I have been created ";
    str += "by "+identify(previous_object(-1));

    if(environment()) str += "\nMy current environment is "+file_name(environment())+".";
    else str += "\nI currently have no environment.";
    str += "\nCall stack: "+get_stack();
    SetNoSink(1);

    enter::create();
    parse_init();
    if( adj ){
        SetAdjectives(adj...);
    }
    if( id ){
        if(objectp(id)) id = id->GetId();
        SetKeyName(id[0]);
        if(!GetShort()) SetShort(add_article(id[0]));
        SetId(id...);
    }
    SetExternalDesc(long || "");
    SetInvis(1);
}

varargs mixed eventKnock(object who, mixed what){
    return enter::eventKnock(who, what);
}

varargs mixed eventScratch(object who, mixed what){
    return enter::eventScratch(who, what);
}
