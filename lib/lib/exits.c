/*    /lib/room/exits.c
 *    from the Dead Souls LPC Library
 *    handles players exiting from rooms
 *    created by Descartes of Borg 940711
 *    modified for new inheritance structure by Descartes 950208
 *    Version: @(#) exits.c 1.12@(#)
 *    Last Modified: 96/12/23
 */

#include <lib.h>
#include <rooms.h>
#include <position.h>
#include "include/exits.h"

private static string Obvious, GoMessage, EnterMessage, Dir, Sky;
private static mapping Exits, Doors;

static void create() {
    Exits = ([]);
    Doors = ([]);
    Obvious = "";
    Dir = "/" + implode(explode(file_name(), "/")[0..<2], "/");
    GoMessage = "You go nowhere at all.\n";
    EnterMessage = "You can't enter that!\n";
}

mixed CanGo(object who, string str) {
    if( (int)who->GetParalyzed() ) return "You are unable to move.";
    if( !Exits[str] ) return GoMessage;
    else return 1;
}

mixed eventGo(object who, string str) {
    if( who->GetPosition() != POSITION_STANDING ) {
	who->eventStand();
	if( who->GetPosition() != POSITION_STANDING ) {
	    return 0;
	}
    }
    if( Doors[str] && (int)Doors[str]->GetClosed() ) {
	message("my_action", "You bump into " + 
		(string)Doors[str]->GetShort(str) + ".", who);
	return 1;
    }
    if( Exits[str]["pre"] && !((int)evaluate(Exits[str]["pre"], str)) )
      return 1;
    who->eventMoveLiving(Exits[str]["room"], str);
    if( Exits[str]["post"] ) evaluate(Exits[str]["post"], str);
    return 1;
}

string GetDoor(string dir) {
    return Doors[dir];
}

string array GetDoors() {
    return keys(Doors);
}

string SetDoor(string dir, string file) {
    object ob = GetDummyItem(dir);

    if( ob ) {
	ob->SetDoor(file);
    }
    file->eventRegisterSide(dir);
    return (Doors[dir] = file); 
}

string GetDirection(string dest) {
    string *cles;
    int i;

    foreach(string dir, mapping data in Exits) {
	if( data["room"] == dest ) {
	    return "go " + dir;
	}
    }
    foreach(string dir in GetEnters()) {
	mapping data = GetEnterData(dir);

	if( data["room"] == dest ) {
	    return "enter " + dir;
	}
    }
    return 0;
}

object GetDummyItem(mixed id) {
    object array items = filter(all_inventory(), (: $1->isDummy() :));
    
    if( stringp(id) ) {
	id = ({ id });
    }
    foreach(object item in items) {
	if( sizeof(item->GetId() & id) ) {
	    return item;
	}
    }
    return 0;
}
    
varargs void AddEnter(string dir, string dest, function pre, function post) {
    object ob = GetDummyItem(dir);

    ob->SetEnter(dest, pre, post);
}

string GetEnter(string dir) {
    object ob = GetDummyItem(dir);

    if( !ob ) {
	return 0;
    }
    else {
	return ob->GetEnter();
    }
}

static mapping GetEnterData(string dir) {
    object ob = GetDummyItem(dir);

    if( !ob ) {
	return 0;
    }
    else {
	return ob->GetEnterData();
    }
}

string array GetEnters() {
    object array obs = filter(all_inventory(), (: $1->isDummy() :));
    string array ids = ({});

    foreach(object ob in obs) {
	if( ob->GetEnter() ) {
	    ids += ob->GetId();
	}
    }
    return ids;
}

void RemoveEnter(string dir) {
    object ob = GetDummyItem(dir);

    ob->SetEnter(0);
}

void SetEnters(mapping mp) {
    foreach(mixed dir, mixed room_or_arr in mp) {
	object ob = GetDummyItem(dir);

	if( arrayp(room_or_arr) ) {
	    ob->SetEnter(room_or_arr...);
	}
	else {
	    ob->SetEnter(room_or_arr);
	}
    }
}

string GetEnterMessage() {
    return EnterMessage[0..<2];
}

string SetEnterMessage(string str) {
    return (EnterMessage = str + "\n");
}

varargs mapping AddExit(string dir, string dest, function pre, function post) {
    if(!stringp(dir)) error("Bad argument 1 to AddExit().\n");
    if(!stringp(dest)) error("Bad argument 2 to AddExit().\n");
    dest = ResolveObjectName(dest);
    Exits[dir] = ([ "room" : dest  ]);
    if( functionp(pre) ) Exits[dir]["pre"] = pre;
    if( functionp(post) ) Exits[dir]["post"] = post;
    return Exits[dir];
}

string GetExit(string str) {
    if(!Exits[str]) return 0;
    else return Exits[str]["room"];
}

static mapping GetExitData(string str) {
    return Exits[str];
}

string array GetExits() {
    return keys(Exits);
}

mapping RemoveExit(string dir) {
    if(Exits[dir]) map_delete(Exits, dir);
}

mapping SetExits(mapping mp) {
    mixed room_or_arr, dir;

    Exits = ([]);
    foreach(dir, room_or_arr in mp) {
	if( arrayp(dir) ) {
	    string real_dir;

	    foreach(real_dir in dir) {
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

string GetGoMessage() {
    return GoMessage[0..<2];
}

string SetGoMessage(string str) {
    return (GoMessage = str + "\n");
}

string GetObviousExits() {
    return Obvious;
}

string SetObviousExits(string str) {
    return (Obvious = str);
}

string GetSky() {
    return Sky;
}

string SetSky(string str) {
    return (Sky = str);
}

string ResolveObjectName(string file) {
    if( file[<2..] == ".c" ) file = file[0..<3];
    return absolute_path(Dir, file);
}
