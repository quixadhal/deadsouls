/*    /lib/std/limb.c
 *    From the Dead Souls Object Library
 *    Standard limb object for severed limbs
 *    Created by Descartes of Borg 950329
 *    Version: @(#) limb.c 1.5@(#)
 *    Last Modified: 96/12/31
 */

#include <lib.h>

inherit LIB_STORAGE;

private int Count          = 0;
private int DecayLife      = 100;
private string Limb        = 0;
private string Owner       = 0;
private string Race        = 0;
private static int CallOut = -1;

int eventDecay();

int GetDecayLife() {
    return DecayLife;
}

int SetDecayLife(int x) {
    return (DecayLife = x);
}

string GetLimb() {
    return Limb;
}

void SetLimb(string limb, string owner, string race) {
    SetKeyName("limb");
    SetId( ({ "limb", Limb = limb }) );
    Owner = owner;
    Race = race;
    Count = 1;
    SetShort("a rotting " + possessive_noun(Race) + " " + Limb);
    SetLong("This limb has a horrible stench as it rots to nothing.");
    CallOut = call_out((: eventDecay :), DecayLife/3);
}

string GetOwner() {
    return Owner;
}

string GetRace() {
    return Race;
}

int GetSaveString() {
    return 0;
}

string GetShort() {
    string str = storage::GetShort();

    if( !str ) {
	str = "a limb";
    }
    return str;
}

int CanReceive(object ob) {
    return 1;
}

int Destruct() {
    if( CallOut > -1 ) {
	remove_call_out(CallOut);
    }
    return storage::Destruct();
}

int eventDecay() {
    if( !environment() ) {
	CallOut = -1;
	Destruct();
	return 0;
    }
    switch(Count) {
        case 1:
	    message("smell", "The "+Limb+" really stinks.", environment());
	    SetShort("the stinky remains of a rotting " + Limb);
	    break;
	case 2:
	    message("smell", "A rotting stench fills the entire area.",
		    environment());
	    SetShort("a pile of rotting flesh");
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
