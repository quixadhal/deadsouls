/*    /lib/door.c
 *    from the Dead Souls LPC Library
 *    the standard door, window, rock object
 *    created by Descartes of Borg 950410
 *    with many thanks to Balack@Phoenix and Deathblade@ZorkMUD for their ideas
 *    Version: @(#) door.c 1.8@(#)
 *    Last modified: 97/01/01
 */

#include <lib.h>
#include "include/door.h"

inherit LIB_DAEMON;
inherit LIB_SEAL;

private mapping Sides;

/*  ***************  /lib/door.c driver applies  ***************  */

static void create() {
    daemon::create();
    parse_init();
    SetNoClean(1);
    SetLockStrength(50);
    Sides = ([]);
}

/*  ***************  /lib/door.c modals  **************  */
/* method: CanLock()
 * args:
 *    object who - the person doing the locking
 *    object key - the key being used for the locking
 *
 * returns: 1 on success, 0 or failure string on failure
 *
 * Tests to see if a certain player can lock the door with a given
 * key
 */
mixed CanLock(object who) {
    object room;

    if( !(room = environment(who)) ) return 0;
    foreach(string side, class door_side val in Sides) {
	if( member_array(room, val->Rooms) != -1 ) {
	    if( !GetLockable(side) ) return 0;
	    else return seal::CanLock(who);
	}
    }
    return 0;
}

/* method: CanUnlock()
 * args:
 *    object who - the person doing the unlocking
 *    object key - the key being used for the unlocking
 *
 * returns: 1 on success, 0 or failure string on failure
 *
 * Tests to see if a certain player can unlock the door with a given
 * key
 */
mixed CanUnlock(object who) {
    object room;

    if( !(room = environment(who)) ) return 0;
    foreach(string side, class door_side val in Sides) {
	if( member_array(room, val->Rooms) != -1 ) {
	    if( !GetLockable(side) ) return 0;
	    else return seal::CanUnlock(who);
	}
    }
    return 0;
}

/*  ***************  /lib/door.c events  **************  */

/* eventClose()
 * object who - who is closing it
 *
 * returns 1,0, or an error string based on parser conditions
 *
 * Check to be certain the door is not already closed before calling so
 * you may issue the appropriate message
 */

varargs mixed eventClose(object who) {
    object room;
    string tmp;
    
    SetClosed(1);
    room = environment(who);
    foreach(string side, class door_side val in Sides) {
	if( member_array(environment(who), val->Rooms) != -1 ) tmp = side;
	filter(val->Rooms, (: $1 && ($1 != $(room)):))->eventPrint(capitalize(GetShort(side)) + " closes.");
    }
    who->eventPrint("You close " + GetShort(tmp) + ".");
    room->eventPrint((string)who->GetName() + " closes " + GetShort(tmp) + ".",
		     who);
    return 1;
}

/* eventLock()
 * object who - the agent responsible for locking it
 * object key - the key being used to lock it
 * returns 1, 0 or a string according to parser conditions
 *
 * Check to make sure the door is closed and unlocked before calling this
 * Also check to see that the door is lockable from this side
 * object by is required here
 */

mixed eventLock(object who, object key) {
    object room;
    
    room = environment(who);
    foreach(string side, class door_side val in Sides) {
	if( member_array(room, val->Rooms) != -1 ) {
	    string tmp;

	    tmp = GetShort(side);
	    if( !(sizeof(key->GetId() & GetKeys(side))) ) {
		who->eventPrint("You fail to lock " + tmp + ".");
		room->eventPrint((string)who->GetName() + " attempts to " 
				 "lock " + tmp + " with " +
				 (string)key->GetShort() + ", but fails.",who);
		return 1;
	    }
	    SetLocked(1);
	    who->eventPrint("You lock " + tmp + ".");
	    room->eventPrint((string)who->GetName() + " locks " + tmp +
			     " with " + (string)key->GetShort() + ".", who);
	    return 1;
	}
    }
    return 0;
}

/* eventOpen()
 * object by - The room from which the door is being open
 * object agent - The living thing responsible for it opening
 *
 * returns 1, 0, or an error string depending on parser circumstances
 *
 * Used for actually opening the door
 * Before calling this, you should check if the door is locked and
 * check if it is already open so you can issue the appropriate messages
 */

varargs int eventOpen(object who, object tool) {
    object room;
    string tmp;
    
    if( tool ) return seal::eventOpen(who, tool);
    SetClosed(0);
    room = environment(who);
    foreach(string side, class door_side val in Sides) {
	if( member_array(environment(who), val->Rooms) != -1 ) tmp = side;
	filter(val->Rooms, (: $1 && ($1 != $(room)) :))->eventPrint(capitalize(GetShort(side)) + " opens.");
    }
    who->eventPrint("You open " + GetShort(tmp) + ".");
    room->eventPrint((string)who->GetName() + " opens " + GetShort(tmp) + ".",
		     who);
    return 1;
}

/* eventRegisterSide()
 * string side - directional exit identifying which side of the door this room
 *               exits in
 *
 * returns 1 on success, 0 on failure
 *
 * never should be called manually
 * this is called by SetDoor() in exits.c to tell the door there is a room
 * which is observing it
 */
 
int eventRegisterSide(string side) {
    string array id = GetId(side);
    
    if( !Sides[side] ) return 0;
    ((class door_side)Sides[side])->Rooms = 
      distinct_array(((class door_side)Sides[side])->Rooms +
		     ({ previous_object() }));
    previous_object()->AddItem(id, (: GetLong($(side)) :));
    foreach(object ob in all_inventory(previous_object())) {
	if( !ob->isDummy() ) {
	    continue;
	}
	if( sizeof(id & ob->GetId()) ) {
	    ob->SetDoor(file_name(this_object()));
	}
    }
    return 1;
}

/* method: eventUnlock()
 * args:
 *    object who - the person unlocking the door
 *    object key - the key being used to unlock it
 *
 * returns: 1 on success, 0 or failure string on failure
 *
 * Check to make sure the door is closed and locked before calling this
 * Also check to see that the door is lockable from this side
 * object by is required here
 */
mixed eventUnlock(object who, object key) {
    object room;
    
    room = environment(who);
    foreach(string side, class door_side val in Sides) {
	if( member_array(room, val->Rooms) != -1 ) {
	    string tmp;

	    tmp = GetShort(side);
	    if( !sizeof((string *)key->GetId() & GetKeys(side)) ) {
		who->eventPrint("You fail to unlock " + tmp + ".");
		room->eventPrint((string)who->GetName() + " attempts to "
				 "unlock " + tmp + " with " +
				 (string)key->GetShort() + ", but fails.",who);
		return 1;
	    }
	    SetLocked(0);
	    who->eventPrint("You unlock " + tmp + ".");
	    room->eventPrint((string)who->GetName() + " unlocks " + tmp +
			     " with " + (string)key->GetShort() + ".", who);
	    return 1;
	}
    }
    return 0;
}

/*  **************  /lib/door.c data functions  **************  */

void SetSide(string side, mapping mp) {
    class door_side new_side;
    
    new_side = new(class door_side);
    new_side->Rooms = ({});
    if( stringp(mp["id"]) ) new_side->Ids = ({ mp["id"] });
    else new_side->Ids = mp["id"];
    new_side->Short = mp["short"];
    new_side->Long = mp["long"];
    if( stringp(mp["keys"]) ) new_side->Keys = ({ mp["keys"] });
    else new_side->Keys = mp["keys"];
    new_side->Lockable = mp["lockable"];
    Sides[side] = new_side;
}

int SetLockable(string side, int x) {
    if( !Sides[side] )
      Sides[side] = new(class door_side, Rooms : ({}));
    return (((class door_side)Sides[side])->Lockable = x); 
}

int GetLockable(string side) {
    return ((class door_side)Sides[side])->Lockable;
}

varargs string *SetId(string side, mixed *args...) { 
    if( !Sides[side] ) Sides[side] = new(class door_side, Rooms : ({}));
    ((class door_side)Sides[side])->Ids = ({});
    foreach(mixed val in args) {
	if( stringp(val) ) ((class door_side)Sides[side])->Ids += ({ val });
	else ((class door_side)Sides[side])->Ids += val;
    }
    return ((class door_side)Sides[side])->Ids;
}

string *GetId(string side) { return ((class door_side)Sides[side])->Ids; }

mixed SetShort(string side, mixed short) {
    if( !Sides[side] )
      Sides[side] = new(class door_side, Rooms : ({}));
    return (((class door_side)Sides[side])->Short = short);
}

varargs string GetShort(string side) {
    if( !side) { /* let's hack a side */
	object room;

	if( !this_player() ) room = previous_object();
	else room = environment(this_player());
	foreach(string s, class door_side val in Sides) {
	    side = s;
	    if( member_array(room, val->Rooms) != -1 ) break;
	}
    }
    if( stringp(((class door_side)Sides[side])->Short) )
      return ((class door_side)Sides[side])->Short;
    else return (string)evaluate(((class door_side)Sides[side])->Short, side);
}

string GetDefiniteShort() {
    string tmp = GetShort();

    return add_article(tmp, 1);
}

mixed SetLong(string side, mixed long) {
    if( !Sides[side] )
      Sides[side] = new(class door_side, Rooms : ({}));
    return (((class door_side)Sides[side])->Long = long);
}

string GetLong(string side) {
    string tmp;

    if( GetClosed() ) tmp = "It is closed.";
    else tmp = "It is open.";
    if( stringp(((class door_side)Sides[side])->Long) )
      return ((class door_side)Sides[side])->Long + "\n" + tmp;
    else return (string)evaluate(((class door_side)Sides[side])->Long, side);
}

varargs string *SetKeys(string side, mixed *args...) {
    if( !Sides[side] ) Sides[side] = new(class door_side, Rooms : ({}));
    ((class door_side)Sides[side])->Keys = ({});
    foreach(mixed val in args) {
	if( stringp(val) ) ((class door_side)Sides[side])->Keys += ({ val });
	else ((class door_side)Sides[side])->Keys += val;
    }
    return ((class door_side)Sides[side])->Keys;
}

string *GetKeys(string side) { return ((class door_side)Sides[side])->Keys; }

object *GetRooms(string side) { return ((class door_side)Sides[side])->Rooms; }
