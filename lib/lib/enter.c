/*    /lib/enter.c
 *    From the Dead Souls V Object Library
 *    Allows objects to provide an entry portal into another place
 *    Created by Descartes of Borg 961015
 *    Version: @(#) enter.c 1.4@(#)
 *    Last modified: 97/01/02
 */

#include <position.h>

private string Dir;
private string Door = 0;
private mapping Enter = 0;

static void create() {
    Dir = "/" + implode(explode(file_name(), "/")[0..<2], "/");
}

string ResolveObjectName(string file) {
    if( file[<2..] == ".c" ) file = file[0..<3];
    return absolute_path(Dir, file);
}

mixed direct_close_obj(object target) {
  if( !Door ) {
      return 0;
  }
  else {
      return Door->CanClose(this_player());
  }
}

mixed direct_enter_obj() {
    if( !Enter ) {
	return environment()->GetEnterMessage();
    }
    else {
	return 1;
    }
}

mixed direct_enter_into_obj() {
    return direct_enter_obj();
}

varargs mixed direct_lock_obj_with_obj(object target, object tool) {
    if( !Door ) {
	return 0;
    }
    else {
	return Door->CanLock(this_player());
    }
}

mixed direct_open_obj(object target) {
    if( !Door ) {
	return 0;
    }
    else {
	return Door->CanOpen(this_player());
    }
}

mixed direct_open_obj_with_obj(object target, object ob) {
    return direct_open_obj(target);
}

mixed direct_pick_str_on_obj(string str, object ob, string id1, string id2) {
    if( !Door || remove_article(lower_case(str)) != "lock" ) {
	return 0;
    }
    else {
	return Door->CanPick(this_player(), remove_article(lower_case(id2)));
    }
}

mixed direct_pick_str_on_obj_with_obj(string str, object ob, object w,
				      string id1, string id2, string id3) {
    return direct_pick_str_on_obj(str, ob, id1, id2);
}

mixed direct_unlock_obj_with_obj(object target, object w) {
    if( !Door ) {
	return 0;
    }
    else {
	return Door->CanUnlock(this_player());
    }
}

mixed eventClose(object who) {
    return Door->eventClose(who);
}

mixed eventEnter(object who, string what) {
    if( Door && Door->GetClosed() ) {
	who->eventPrint("You bump into " + Door->GetShort(what) + ".");
	environment(who)->eventPrint(who->GetName() + " bumps into " +
				     Door->GetShort(what) + ".", who);
	return 1;
    }
    if( who->GetPosition() != POSITION_STANDING ) {
	who->eventStand();
	if( who->GetPosition() != POSITION_STANDING ) {
	    return 0;
	}
    }
    if( Enter["pre"] && !evaluate(Enter["pre"], what) ) {
	return 1;
    }
    who->eventMoveLiving(Enter["room"], "$N enters into the " + what + ".");
    if( Enter["post"] ) {
	evaluate(Enter["post"], what);
    }
    return 1;
}

mixed eventLock(object who, object key) {
    Door->eventLock(who, key);
}

varargs mixed eventOpen(object who, object tool) {
    return Door->eventOpen(who, tool);
}

varargs mixed eventPick(object who, string str, object tool) {
    return Door->eventPick(who, str, tool);
}

mixed eventUnlock(object who, object key) {
    return Door->eventUnlock(who, key);
}

string GetDoor() {
    return Door;
}

void SetDoor(string door) {
    Door = door;
}

varargs void SetEnter(string dest, function pre, function post) {
    if( !dest ) {
	Enter = 0;
	return;
    }
    dest = ResolveObjectName(dest);
    Enter = ([ "room" : dest, "pre" : pre, "post" : post ]);
}

string GetEnter() {
    if( !Enter ) {
	return 0;
    }
    else {
	return Enter["room"];
    }
}

mapping GetEnterData() {
    return Enter;
}

