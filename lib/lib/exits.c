/*    /lib/room/exits.c
 *    from the Dead Souls LPC Library
 *    handles players exiting from rooms
 *    created by Descartes of Borg 940711
 *    modified for new inheritance structure by Descartes 950208
 *    Version: @(#) exits.c 1.12@(#)
 *    Last Modified: 96/12/23
 */

#include <lib.h>
#include ROOMS_H
#include <daemons.h>
#include <position.h>
#include "include/exits.h"

private static string Obvious, GoMessage, EnterMessage, Dir, Sky;
private static mapping Exits, Doors;

static void create(){
    Exits = ([]);
    Doors = ([]);
    Obvious = "";
    Dir = "/" + implode(explode(file_name(), "/")[0..<2], "/");
    GoMessage = "You go nowhere at all.\n";
    EnterMessage = "You can't enter that!\n";
}

mixed CanGo(object who, string str){
    int noclip;
    if( who->GetParalyzed() ) return "You are unable to move.";
    noclip = who->GetProperty("noclip");
    if( !noclip && !Exits[str] && str != "up" && str != "down" &&
            !(sizeof(this_object()->GetFlyRoom())) &&
            !(sizeof(this_object()->GetSinkRoom())) ){
        return GoMessage;
    }
    else return 1;
}

mixed eventGo(object who, string str){
    int noclip = who->GetProperty("noclip");
    if(query_verb() == "go" && interactive(who) && !noclip){	
        if( who->GetPosition() != POSITION_STANDING ){  
            write("You are not standing.");
            switch(who->GetPosition()){
                case POSITION_LYING : write("Try: crawl "+str);break; 
                case POSITION_SITTING : write("Try: crawl "+str);break; 
                case POSITION_KNEELING : write("Try: crawl "+str);break; 
                case POSITION_FLOATING : write("You are floating.");break; 
                case POSITION_SWIMMING : write("Try: swim "+str);break; 
                case POSITION_FLYING : write("Try: fly "+str);break; 
            }
            return 0;
        }
    }
    else if(query_verb() == "crawl"){
        if( who->GetPosition() != POSITION_LYING &&
                who->GetPosition() != POSITION_KNEELING &&
                who->GetPosition() != POSITION_SITTING ){
            write("You are not in the correct position for crawling.");
            return 0;
        }
    }
    else if(query_verb() == "fly"){
        if( who->GetPosition() != POSITION_FLYING ){
            write("You are not flying.");
            return 0;
        }
    }
    else if(query_verb() == "swim"){
        if( who->GetPosition() != POSITION_SWIMMING ){
            write("You are not swimming.");
            return 0;
        }
    }

    if(!noclip && sizeof(Doors) && Doors[str] && Doors[str]->GetClosed() ){
        message("my_action", "You bump into " + 
                Doors[str]->GetShort(str) + ".", who);
        return 1;
    }
    if(!noclip && Exits[str] && Exits[str]["pre"] && 
            !(evaluate(Exits[str]["pre"], str)) )
        return 1;
    if(!Exits[str]){
        if( str == "up" && sizeof(this_object()->GetFlyRoom())){
            if(noclip || who->GetPosition() == POSITION_FLYING){
                string omsg = who->GetName()+" flies up.";
                string imsg = who->GetName()+" flies in.";
                who->eventMoveLiving(this_object()->GetFlyRoom(),omsg,imsg,str);
                return 1;
            }
        }

        if(str == "down" && sizeof(this_object()->GetSinkRoom())){
            string omsg = who->GetName()+" sinks down.";
            string imsg = who->GetName()+" sinks in.";
            who->eventMoveLiving(this_object()->GetSinkRoom(),omsg,imsg,str);
            return 1;
        }
        if(!noclip){ 
            write("You can't go that way.");
            return 0;
        }
    }
    if(Exits[str] && Exits[str]["room"]){
        who->eventMoveLiving(Exits[str]["room"],0,0,str);
        if( Exits[str]["post"] ) evaluate(Exits[str]["post"], str);
        return 1;
    }
    if(noclip){
        int moved;
        string ndest = ROOMS_D->GetDirectionRoom(this_object(),str,1);
        if(!ndest){
            write("You cannot noclip in that direction.");
        }
        moved = who->eventMoveLiving(ndest,0,0,str);
        if(base_name(environment(who)) != ndest){
            write("You fail to noclip in that direction.");
        }
        return 1;
    }
    return 0;
}

mixed GetDoor(string dir){
    if(sizeof(Doors)) return Doors[dir];
    else return 0;
}

string array GetDoors(){
    return keys(Doors);
}

mapping GetDoorsMap(){
    return copy(Doors);
}

string SetDoor(string dir, string file){
    object ob = GetDummyItem(dir);

    if( ob ){
        ob->SetDoor(file);
    }

    if(!unguarded( (: file_exists($(file)) :) ) && 
            !unguarded( (: file_exists($(file)+".c") :) )){
        return "Door not found.";
    }
    file->eventRegisterSide(dir);
    return (Doors[dir] = file); 
}

varargs string CreateDoor(string dir, string odir, string long, string locked, string key){
    object new_door = new(LIB_DOOR);
    string doorfile = file_name(new_door);
    object ob = GetDummyItem(dir);
    if(!locked) locked = "";
    if( ob ){
        ob->SetDoor(doorfile);
    }
    new_door->SetSide(dir, ([ "id" : ({"door", dir+" door", long}), "short" : "a door leading "+dir, "long" : long, "lockable" : 1 ]));
    new_door->SetSide(odir, ([ "id" : ({"door", odir+" door", long}), "short" : "a door leading "+odir, "long" : long, "lockable" : 1 ]));
    if(key) new_door->SetKeys(dir, ({ key }) );
    if(key) new_door->SetKeys(odir, ({ key }) );
    new_door->eventRegisterSide(dir);
    return (Doors[dir] = doorfile);
}

string GetDirection(string dest){
    foreach(string dir, mapping data in Exits){
        if( data["room"] == dest ){
            return "go " + dir;
        }
    }
    foreach(string dir in GetEnters()){
        string data = GetEnter(dir);

        if(data && data  == dest ){
            return "enter " + dir;
        }
    }
    return 0;
}

object GetDummyItem(mixed id){
    int i;
    object array dummies,all_inv;

    all_inv=all_inventory();
    dummies = ({});

    for(i=0; i<sizeof(all_inv); i++){
        if ( all_inv[i]->isDummy() )  dummies += ({ all_inv[i] });
    }
    if( stringp(id) ){
        id = ({ id });
    }
    if(arrayp(id)){
        foreach(object dummy in dummies){
            foreach(string element in id){
                if(answers_to(element,dummy)) return dummy;
            }
        }
    }
    return 0;
}

varargs void AddEnter(string dir, string dest, function pre, function post){
    object ob = GetDummyItem(dir);

    ob->SetEnter(dest, pre, post);
}

string GetEnter(string dir){
    object ob = GetDummyItem(dir);

    if( !ob ){
        return 0;
    }
    else {
        return ob->GetEnter();
    }
}

static mapping GetEnterData(string dir){
    object ob = GetDummyItem(dir);

    if( !ob ){
        return 0;
    }
    else {
        return ob->GetEnterData();
    }
}

varargs string array GetEnters(int i){
    object *obs;
    string *ids;

    obs = ({});
    ids = ({});

    foreach(object item in all_inventory(this_object())){
        if(item->isDummy()){
            obs += ({ item });
        }
    }

    foreach(object ob in obs){
        if( ob->GetEnter() ){
            if(i) ids += ({ ob->GetKeyName() });
            else ids += ob->GetId();
        }
    }
    return ids;
}

mapping GetEnterMap(){
    mixed schlussel;
    mapping EnterMap = ([]);
    object *obs = ({});
    foreach(object item in all_inventory(this_object())){
        if(item->isDummy()){
            obs += ({ item });
        }
    }
    if(!sizeof(obs)) return ([]);
    foreach(object ob in obs){
        if( ob->GetEnter() ){
            schlussel = ob->GetId();
            EnterMap[schlussel] = ob->GetEnter();
        }
    }
    return copy(EnterMap);
}

void RemoveEnter(string dir){
    object ob = GetDummyItem(dir);

    ob->SetEnter(0);
}

void SetEnters(mapping mp){
    foreach(mixed dir, mixed room_or_arr in mp){
        object ob = GetDummyItem(dir);
        if(!ob) continue;

        if( arrayp(room_or_arr) ){
            ob->SetEnter(room_or_arr...);
        }
        else {
            ob->SetEnter(room_or_arr);
        }
    }
}

string GetEnterMessage(){
    return EnterMessage[0..<2];
}

string SetEnterMessage(string str){
    return (EnterMessage = str + "\n");
}

varargs mapping AddExit(string dir, string dest, function pre, function post){
    if(!stringp(dir)) error("Bad argument 1 to AddExit().\n");
    if(!stringp(dest)) error("Bad argument 2 to AddExit().\n");
    dest = ResolveObjectName(dest);
    Exits[dir] = ([ "room" : dest  ]);
    if( functionp(pre) ) Exits[dir]["pre"] = pre;
    if( functionp(post) ) Exits[dir]["post"] = post;
    return Exits[dir];
}

string GetExit(string str){
    if(!Exits[str]) return 0;
    else return Exits[str]["room"];
}

mapping GetExitData(string str){
    return Exits[str];
}

mapping GetFullExitData(){
    return Exits;
}

mapping GetExitMap(){
    mapping ret = ([]);
    foreach(string key in keys(Exits)){
        ret[key] = Exits[key]["room"];
    }  

    return ret;
}

string array GetExits(){
    return keys(Exits);
}

mapping RemoveExit(string dir){
    if(Exits[dir]) map_delete(Exits, dir);
}

mapping SetExits(mapping mp){
    mixed room_or_arr, dir;

    Exits = ([]);
    foreach(dir, room_or_arr in mp){
        if( arrayp(dir) ){
            string real_dir;

            foreach(real_dir in dir){
                if( arrayp(room_or_arr) ) AddExit(real_dir, room_or_arr...);
                else AddExit(real_dir, room_or_arr);
            }
        }
        else {
            if( stringp(room_or_arr) ) AddExit(dir, room_or_arr);
            else if( arrayp(room_or_arr) ) AddExit(dir, room_or_arr...);
        }
    }
    return Exits;
}

string GetGoMessage(){
    return GoMessage[0..<2];
}

string SetGoMessage(string str){
    return (GoMessage = str + "\n");
}

string GetObviousExits(){
    return Obvious;
}

string SetObviousExits(string str){
    return (Obvious = str);
}

string GetSky(){
    return Sky;
}

string SetSky(string str){
    return (Sky = str);
}

string ResolveObjectName(string file){
    if( file[<2..] == ".c" ) file = file[0..<3];
    return absolute_path(Dir, file);
}
