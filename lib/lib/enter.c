/*    /lib/enter.c
 *    From the Dead Souls Object Library
 *    Allows objects to provide an entry portal into another place
 *    Created by Descartes of Borg 961015
 *    Version: @(#) enter.c 1.4@(#)
 *    Last modified: 97/01/02
 */

#include <position.h>

private string Dir;
private string Door, DoorSide = 0;
private mapping Enter = 0;

static void create(){
    Dir = "/" + implode(explode(file_name(), "/")[0..<2], "/");
}

string ResolveObjectName(string file){
    if( file[<2..] == ".c" ) file = file[0..<3];
    return absolute_path(Dir, file);
}

mixed direct_close_obj(object target){
    if( !Door ){
        return 0;
    }
    else {
        return Door->CanClose(this_player());
    }
}

mixed direct_enter_obj(){
    if( !Enter ){
        return environment()->GetEnterMessage();
    }
    else {
        return 1;
    }
}

mixed direct_enter_into_obj(){
    return direct_enter_obj();
}

varargs mixed direct_lock_obj_with_obj(object target, object tool){
    if( !Door ){
        return 0;
    }
    else {
        return Door->CanLock(this_player());
    }
}

mixed direct_open_obj(object target){
    if( !Door ){
        return 0;
    }
    else {
        return Door->CanOpen(this_player());
    }
}

mixed direct_open_obj_with_obj(object target, object ob){
    return direct_open_obj(target);
}

mixed direct_pick_str_on_obj(string str, object ob, string id1, string id2){
    if( !Door || remove_article(lower_case(str)) != "lock" ){
        return 0;
    }
    else {
        return Door->CanPick(this_player(), remove_article(lower_case(id2)));
    }
}

mixed direct_pick_str_on_obj_with_obj(string str, object ob, object w,
        string id1, string id2, string id3){
    return direct_pick_str_on_obj(str, ob, id1, id2);
}

mixed direct_unlock_obj_with_obj(object target, object w){
    if( !Door ){
        return 0;
    }
    else {
        return Door->CanUnlock(this_player());
    }
}

mixed eventClose(object who){
    return Door->eventClose(who);
}

varargs mixed eventEnter(object who, string what, string verb){
    if(!verb) verb = "NOVERB";
    if( Door && Door->GetClosed() ){
        who->eventPrint("You bump into " + Door->GetShort(what) + ".");
        environment(who)->eventPrint(who->GetName() + " bumps into " +
                Door->GetShort(what) + ".", who);
        return 1;
    }
    if( who->GetPosition() != POSITION_STANDING ){
        if(verb != "crawl" && verb != "fly") who->eventStand();
        if( who->GetPosition() != POSITION_STANDING && verb != "crawl" 
                && verb != "fly"){
            who->eventPrint("weird.");
            return 0;
        }
    }
    if( Enter["pre"] && !evaluate(Enter["pre"], what) ){
        return 1;
    }
    if(verb == "crawl") who->eventMoveLiving(Enter["room"],"into the " + this_object()->GetKeyName() );
    else who->eventMoveLiving(Enter["room"], "$N enters into the " + this_object()->GetKeyName() + ".");
    if( Enter["post"] ){
        evaluate(Enter["post"], what);
    }
    return 1;
}

varargs mixed eventLock(object who, mixed arg1, mixed arg2){
    object key;
    if(arg1 && objectp(arg1)) key = arg1;
    else if(arg2 && objectp(arg2)) key = arg2;
    else return 0;
    return Door->eventLock(who, key);
}

varargs mixed eventOpen(object who, object tool){
    return Door->eventOpen(who, tool);
}

varargs mixed eventKnock(object who, mixed what){
    if(!Door){
        write("You can't knock on that.");
        return 0;
    }
    return Door->eventKnock(who, what);
}

varargs mixed eventScratch(object who, object tool){
    return Door->eventScratch(who, tool);
}

varargs mixed eventPick(object who, string str, object tool){
    return Door->eventPick(who, str, tool);
}

mixed eventUnlock(object who, object key){
    return Door->eventUnlock(who, key);
}

string GetDoor(){
    return Door;
}

varargs void SetDoor(string door, string side){
    Door = door;
    if(side) DoorSide = side;
}

string GetDoorSide(){
    return DoorSide;
}

void SetDoorSide(string doorside){
    DoorSide = doorside;
}

varargs void SetEnter(string dest, function pre, function post){
    if( !dest ){
        Enter = 0;
        return;
    }
    dest = ResolveObjectName(dest);
    Enter = ([ "room" : dest, "pre" : pre, "post" : post ]);
}

string GetEnter(){
    if( !Enter ){
        return 0;
    }
    else {
        return Enter["room"];
    }
}

mixed direct_wizlock_obj(){ return 1; }
mixed direct_wizunlock_obj(){ return 1; }

