/*    /lib/events/lock_with.c
 *    From the Dead Souls V Object Library
 *    The indirect object of locks and unlocks (a key)
 *    Created by Descartes of Borg 960121
 */

private int Disabled      = 0;
private int DisableChance = 50;

// abstract methods
string GetDefiniteShort();
// end abstract methods

int GetDisableChance() {
    return DisableChance;
}

int SetDisableChance(int x) {
    return (DisableChance = x);
}

int GetDisabled() {
    return Disabled;
}

int SetDisabled(int x) {
    return (Disabled = x);
}

mixed array GetSave() {
    return ({ "Disabled", "DisableChance" });
}

mixed eventLockLock(object who, object what) {
    return 1;
}

mixed eventUnlockLock(object who, object what) {
    if( GetDisableChance() > random(100) ) {
	SetDisabled(1);
	who->eventPrint(capitalize(GetDefiniteShort()) + " gets twisted "
			"slightly out of shape as you try to use it..");
	return 1;
    }
    return 1;
}

mixed indirect_lock_obj_with_obj(object target, object key, string id) {
    if( environment() != this_player() ) {
	return "#You don't have " + GetDefiniteShort() + "!";
    }
    if( GetDisabled() ) {
	return capitalize(GetDefiniteShort()) + " is broken.";
    }
    return 1;
}    

mixed indirect_unlock_obj_with_obj(object target, object key, string id) {
    if( environment() != this_player() ) {
	return "#You don't have " + GetDefiniteShort() + "!";
    }
    if( GetDisabled() ) {
	return capitalize(GetDefiniteShort()) + " is broken.";
    }
    return 1;
}
