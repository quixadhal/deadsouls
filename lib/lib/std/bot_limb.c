#include <lib.h>

inherit LIB_LIMB;

static void create() {
    limb::create();
}

int eventDecay() {
    if( !environment() ) {
	CallOut = -1;
	Destruct();
	return 0;
    }
    switch(Count) {
    case 10:
	message("smell", "The "+Limb+" rapidly corrodes.", environment());
	SetShort("the corroding remnant of a " + Limb);
	break;
    case 20:
	message("smell", "An acrid chemical odor fills the area.",
	  environment());
	SetShort("some corroded chemicals");
	break;
    case 30:
	CallOut = -1;
	Destruct();
	return 0;
    }
    Count++;
    //CallOut = call_out((: eventDecay :), DecayLife/3);
    return Count;
}

void SetLimb(string limb, string owner, string race) {
    SetKeyName(limb);
    SetId( ({ "limb", Limb = limb }) );
    Owner = owner;
    Race = race;
    Count = 1;
    SetShort("a melting " + possessive_noun(Race) + " " +Limb);
    SetLong("This limb is rapidly corroding into its component chemicals.");
    //CallOut = call_out((: eventDecay :), DecayLife/3);
}
