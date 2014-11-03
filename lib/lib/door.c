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
inherit LIB_KNOCK;
inherit LIB_SCRATCH;

private mapping Sides;
private static int Hidden = 1;
private static int Opacity = 100;
private static int Perforated = 0;

int GetOpacity(){
    return Opacity;
}

int SetOpacity(int x){
    Opacity = x;
    return Opacity;
}

int GetPerforated(){
    return Perforated;
}

int SetPerforated(int x){
    Perforated = x;
    return Perforated;
}

int CanPeer(){
    if(!GetClosed() || !Opacity) return 1;
    return 0;
}

int SetHiddenDoor(int i){
    if(i) Hidden = 1;
    else Hidden = 0;
    return Hidden;
}

int GetHiddenDoor(){
    return Hidden;
}

string *GetSides(){
    return keys(Sides);
}

/*  ***************  /lib/door.c driver applies  ***************  */

static void create(){
    daemon::create();
    parse_init();
    SetNoClean(0);
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
mixed CanLock(object who, string foo){
    object room;
    if( !(room = environment(who)) ) return 0;
    foreach(string side, mapping val in Sides){
        if( member_array(room, val["Rooms"]) != -1 ){
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
mixed CanUnlock(object who){
    object room;

    if( !(room = environment(who)) ) return 0;
    foreach(string side, mapping val in Sides){
        if( member_array(room, val["Rooms"]) != -1 ){
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

varargs mixed eventClose(object who){
    object room,whom;
    string tmp;

    if(!GetClosed()){
        SetClosed(1);
        if(who) room = environment(who);
        else room = environment(this_player());
        if(!who) whom = this_player();
        else whom = who;
        foreach(string side, mapping val in Sides){
            if( member_array(environment(whom), val["Rooms"]) != -1 ) tmp = side;
            if(who)
                filter(val["Rooms"], (: $1 && ($1 != $(room)):))->eventPrint(capitalize(GetShort(side)) + " closes.");
            else (val["Rooms"])->eventPrint(capitalize(GetShort(side)) + " closes.");
        }
        if(who){
            who->eventPrint("You close " + GetShort(tmp) + ".");
            room->eventPrint(who->GetName() + " closes " + GetShort(tmp) + ".",
                    who);
        }
    }
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

varargs mixed eventLock(object who, mixed key, mixed foo){
    object room;

    room = environment(who);

    foreach(string side, mapping val in Sides){
        if( member_array(room, val["Rooms"]) != -1 ){
            string tmp;

            tmp = GetShort(side);
            if( !(sizeof(key->GetId() & GetKeys(side))) ){
                who->eventPrint("You fail to lock " + tmp +
                        " with " + key->GetShort()+".");
                room->eventPrint(who->GetName() + " attempts to " 
                        "lock " + tmp + " with " +
                        key->GetShort() + ", but fails.",who);
                return 1;
            }
            SetLocked(1);
            who->eventPrint("You lock " + tmp +
                    " with " + key->GetShort()+".");
            room->eventPrint(who->GetName() + " locks " + tmp +
                    " with " + key->GetShort() + ".", who);
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

varargs int eventOpen(object who, object tool){
    object room, whom;
    string tmp;

    if(GetClosed()){
        SetClosed(0);
        if(who) room = environment(who);
        else room = environment(this_player());
        if(!who) whom = this_player();
        else whom = who;
        foreach(string side, mapping val in Sides){
            if( member_array(environment(whom), val["Rooms"]) != -1 ) tmp = side;
            if(who)
                filter(val["Rooms"], (: $1 && ($1 != $(room)) :))->eventPrint(capitalize(GetShort(side)) + " opens.");
            else (val["Rooms"])->eventPrint(capitalize(GetShort(side)) + " opens.");
        }
        if(who){
            who->eventPrint("You open " + GetShort(tmp) + ".");
            room->eventPrint(who->GetName() + " opens " + GetShort(tmp) + ".",
                    who);
        }
    }
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

int eventRegisterSide(string side){
    string array id = GetId(side);
    Sides[side]["Rooms"] = 
        distinct_array(Sides[side]["Rooms"] +
                ({ previous_object() }));
    previous_object()->AddItem(id, (: GetLong($(side)) :));
    foreach(object ob in all_inventory(previous_object())){
        if( !ob->isDummy() ){
            continue;
        }
        if( sizeof(id & ob->GetId()) ){
            if(!ob->GetDoor()) ob->SetDoor(file_name(this_object()), side);
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
mixed eventUnlock(object who, object key){
    object room;
    string *key_id = key->GetId();
    room = environment(who);
    foreach(string side, mapping val in Sides){
        if( member_array(room, val["Rooms"]) != -1 ){
            string tmp;

            tmp = GetShort(side);
            if(!sizeof((key_id & (GetKeys(side) || ({}))))){
                who->eventPrint("You fail to unlock " + tmp +
                        " with " + key->GetShort()+".");
                room->eventPrint(who->GetName() + " attempts to "
                        "unlock " + tmp + " with " +
                        key->GetShort() + ", but fails.",who);
                return 1;
            }
            SetLocked(0);
            who->eventPrint("You unlock " + tmp + ".");
            room->eventPrint(who->GetName() + " unlocks " + tmp +
                    " with " + key->GetShort() + ".", who);
            return 1;
        }
    }
    return 0;
}

/*  **************  /lib/door.c data functions  **************  */

void SetSide(string side, mapping mp){
    mapping new_side = ([ "Rooms" : ({}) ]);

    if( stringp(mp["id"]) ) new_side["Ids"] = ({ mp["id"] });
    else new_side["Ids"] = mp["id"];
    new_side["Short"] = mp["short"];
    new_side["Long"] = mp["long"];
    if( stringp(mp["keys"]) ) new_side["Keys"] = ({ mp["keys"] });
    else new_side["Keys"] = mp["keys"];
    new_side["Lockable"] = mp["lockable"];
    Sides[side] = new_side;
}

mapping GetSide(string side){
    mapping RetMap = ([]);
    if(!sizeof(Sides[side])) return ([]);
    RetMap["id"] = Sides[side]["Ids"];
    RetMap["short"] = Sides[side]["Short"];
    RetMap["long"] = Sides[side]["Long"];
    RetMap["keys"] = Sides[side]["Keys"];
    RetMap["lockable"] = Sides[side]["Lockable"];
    return copy(RetMap);
}

int SetLockable(string side, int x){
    if( !Sides[side] )
        Sides[side] = ([ "Rooms" : ({}) ]);
    return Sides[side]["Lockable"] = x; 
}

int GetLockable(string side){
    return Sides[side]["Lockable"];
}

varargs string *SetId(string side, mixed *args...){ 
    if( !Sides[side] ) Sides[side] = ([ "Rooms" : ({}) ]);
    Sides[side]["Ids"] = ({});
    foreach(mixed val in args){
        if( stringp(val) ) Sides[side]["Ids"] += ({ val });
        else Sides[side]["Ids"] += val;
    }
    return Sides[side]["Ids"];
}

string *GetId(string side){ 
    if(!Sides[side]) return ({});
    return Sides[side]["Ids"];
}

mixed SetShort(string side, mixed short){
    if( !Sides[side] )
        Sides[side] = ([ "Rooms" : ({}) ]);
    return Sides[side]["Short"] = short;
}

varargs string GetShort(string side){
    if( !side){ /* let's hack a side */
        object room;
        side = previous_object()->GetDoorSide();
        if( !this_player() ) room = previous_object();
        else room = environment(this_player());
        foreach(string s, mapping val in Sides){
            if(!side) side = s;
            if( member_array(room, val["Rooms"]) != -1 ) break;
        }
    }
    if( stringp(Sides[side]["Short"]) )
        return Sides[side]["Short"];
    else return evaluate(Sides[side]["Short"], side);
}

string GetDefiniteShort(){
    string tmp = GetShort();

    return add_article(tmp, 1);
}

mixed SetLong(string side, mixed long){
    if( !Sides[side] )
        Sides[side] = ([ "Rooms" : ({}) ]);
    return Sides[side]["Long"] = long;
}

string GetLong(string side){
    string tmp;

    if( GetClosed() ) tmp = "It is closed.";
    else tmp = "It is open.";
    if( stringp(Sides[side]["Long"] ) )
        return Sides[side]["Long"] + "\n" + tmp;
    else return evaluate(Sides[side]["Long"], side);
}

varargs string *SetKeys(string side, mixed *args...){
    if( !Sides[side] ) Sides[side] = ([ "Rooms" : ({}) ]);
    Sides[side]["Keys"] = ({});
    foreach(mixed val in args){
        if( stringp(val) ) Sides[side]["Keys"] += ({ val });
        else Sides[side]["Keys"] += val;
    }
    return Sides[side]["Keys"];
}

string *GetKeys(string side){ return Sides[side]["Keys"]; }

object *GetRooms(string side){ return Sides[side]["Rooms"]; }

int get_closed(){ return GetClosed(); }

varargs mixed eventKnock(object who, mixed what){
    object room,whom;
    string tmp;
    if(GetClosed()){
        if(who) room = environment(who);
        else room = environment(this_player());
        if(!who) whom = this_player();
        else whom = who;
        foreach(string side, mixed val in Sides){
            if( member_array(environment(whom), val["Rooms"]) != -1 ) tmp = side;
            if(who)
                filter(val["Rooms"], (: $1 && ($1 != $(room)):))->eventPrint(
                        "There is a knock at the "+remove_article(GetShort(side)) + ".");
            else (val["Rooms"])->eventPrint(
                    "There is a knock at the "+remove_article(GetShort(side)) + ".");
        }
        if(who){
            who->eventPrint("You knock on the " + remove_article(GetShort(tmp)) + ".");
            room->eventPrint(who->GetName() + " knocks on the " + remove_article(GetShort(tmp)) + ".",
                    who);
        }
    }
    else write("It isn't closed!");
    return 1;
}

varargs mixed eventScratch(object who, mixed what){
    object room,whom;
    string tmp;
    if(GetClosed()){
        if(who) room = environment(who);
        else room = environment(this_player());
        if(!who) whom = this_player();
        else whom = who;
        foreach(string side, mixed val in Sides){
            if( member_array(environment(whom), val["Rooms"]) != -1 ) tmp = side;
            if(who)
                filter(val["Rooms"], (: $1 && ($1 != $(room)):))->eventPrint(
                        "There is a scratch at the "+remove_article(GetShort(side)) + ".");
            else (val["Rooms"])->eventPrint(
                    "There is a scratch at the "+remove_article(GetShort(side)) + ".");
        }
        if(who){
            who->eventPrint("You scratch on the " + remove_article(GetShort(tmp)) + ".");
            room->eventPrint(who->GetName() + " scratches on the " + remove_article(GetShort(tmp)) + ".",
                    who);
        }
    }
    else write("It isn't closed!");
    return 1;
}

void init(){
}
