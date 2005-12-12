/*    /lib/limb.c
 *    from the Nightmare IV LPC Library
 *    standard limb object for severed limbs
 *    created by Descartes of Borg 950329
 *    Version: @(#) limb.c 1.2@(#)
 *    Last Modified: 96/05/27
 */


#include <lib.h>
#include "include/limb.h"

inherit LIB_STORAGE;

private int DecayLife, Count;
private string Limb, Owner, Race;
static private int CallOut;

static void create() {
    storage::create();
    Count = 0;
    DecayLife = 100;
    Limb = 0;
    Owner = 0;
    Race = 0;
    CallOut = -1;
}

int Destruct() {
    if( CallOut > -1 ) remove_call_out(CallOut);
    return ::Destruct();
}

int eventDecay() {
    if( !environment() ) {
	CallOut = -1;
	Destruct();
	return 0;
    }
    switch(Count) {
    case 1:
	message("smell", "The "+Limb+" rapidly corrodes.", environment());
	SetShort("the corroding remnant of a " + Limb);
	break;
    case 2:
	message("smell", "An acrid chemical odor fills the area.",
	  environment());
	SetShort("some corroded chemicals");
	break;
    case 3:
	CallOut = -1;
	Destruct();
	return 0;
    }
    Count++;
    CallOut = call_out((: eventDecay :), DecayLife/3);
    return Count;
}

int SetDecayLife(int x) { return (DecayLife = x); }

int GetDecayLife() { return DecayLife; }

void SetLimb(string limb, string owner, string race) {
    SetKeyName("limb");
    SetId( ({ "limb", Limb = limb }) );
    Owner = owner;
    Race = race;
    Count = 1;
    SetShort("a melting " + possessive_noun(Race) + " " +Limb);
    SetLong("This limb is rapidly corroding into its component chemicals.");
    CallOut = call_out((: eventDecay :), DecayLife/3);
}

string GetLimb() { return Limb; }

string GetOwner() { return Owner; }

string GetRace() { return Race; }

string GetShort() {
    string str = storage::GetShort();

    if( !str ) str = "a limb";
    return str;
}

int GetSaveString() { return 0; }
