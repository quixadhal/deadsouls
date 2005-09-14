/*    /lib/obj/storage.c
 *    From the Dead Souls Object Library
 *    The standard object for storing things
 *    Created by Descartes of Borg 940212
 *    Version: @(#) storage.c 1.7@(#)
 *    Last modified: 96/12/31
 */

#include <lib.h>

inherit LIB_HOLDER;
inherit LIB_ITEM;
inherit LIB_SEAL;

private int CanClose = 1;
private int CanLock  = 0;

int GetCanClose() {
    return CanClose;
}

int SetCanClose(int x) {
    CanClose = x;
}

int GetCanLock() {
    return CanLock;
}

int SetCanLock(int x) {
    return SetCanClose(CanLock = x);
}

void SetKey(string key) { /* for backwards compat */
    SetKeys(key);
} 

int GetOpacity() {
    if( GetClosed() ) {
	return holder::GetOpacity();
    }
    else return 0;
}

int GetRadiantLight(int ambient) {
    return item::GetRadiantLight(ambient) + holder::GetRadiantLight(ambient);
}

static mixed array AddSave(mixed array vars) {
    return item::AddSave(vars);
}

int SetSaveRecurse(int x) {
    return item::SetSaveRecurse(x);
}

mixed CanClose(object who, string id) {
    if( !GetCanClose() ) {
	return 0;
    }
    else {
	return seal::CanClose(who, id);
    }
}

mixed CanGetFrom(object who, object item) {
    mixed tmp = holder::CanGetFrom(who, item);

    if( tmp != 1 ) {
	return tmp;
    }
    if( GetClosed() ) {
	return capitalize(GetDefiniteShort()) + " is closed.";
    }
    return 1;
}

mixed CanLock(object who, string id) {
    if( !GetCanLock() ) {
	return 0;
    }
    else {
	return seal::CanLock(who, id);
    }
}

mixed CanOpen(object who, string id) {
    if( !GetCanClose() ) {
	return 0;
    }
    else {
	return seal::CanOpen(who, id);
    }
}

mixed CanPick(object who, string id) {
    if( !GetCanLock() ) {
	return "It isn't lockable in the first place.";
    }
    else {
	return seal::CanPick(who, id);
    }
}

mixed CanPutInto(object who, object what) {
    mixed tmp = holder::CanPutInto(who, what);

    if( tmp != 1 ) {
	return tmp;
    }
    if( GetClosed() ) {
	return capitalize(GetDefiniteShort()) + " is closed.";
    }
    return 1;
}

varargs mixed CanShowInterior(object who, object target) {
    if( GetClosed() ) {
	return capitalize(GetDefiniteShort()) + " is closed.";
    }
    else return holder::CanShowInterior();
}

mixed CanUnlock(object who, string id, object key) {
    if( !GetCanLock() ) {
	return 0;
    }
    else {
	return ::CanUnlock(who, id, key);
    }
}

int eventReceiveObject(object ob) {
    if( GetClosed() ) {
	return 0;
    }
    return holder::eventReceiveObject(ob);
}

void create() {
    AddSave( ({ "CanClose", "CanLock" }) );
    holder::create();
    item::create();
    ::create();
    SetPreventPut("You can't put this in there.");
}

int inventory_accessible() {
    return seal::inventory_accessible();
}

int inventory_visible() {
    return (seal::inventory_visible() || holder::inventory_visible());
}
