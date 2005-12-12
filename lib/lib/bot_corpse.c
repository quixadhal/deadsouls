#include <lib.h>
#include <message_class.h>

inherit LIB_CORPSE;

private int DecayLife, Count, CallOut, Fresh;
private string Owner, Race;

void create() {
    ::create();
    SetId(({"chassis","body","synthetics"}));
    SetAdjectives(({"melting","corroding"}));
    Count = 0;
    CallOut = 0;
    DecayLife = 100;
    Owner = 0;
    Race = 0;
}

int eventDecay() {
    if( !environment() ) {
	Destruct();
	return 0;
    }
    Fresh = 0;
    switch(Count) {
    case 1:
	environment()->eventPrint(possessive_noun(Owner) + " chassis " +
	  "begins to corrode.", MSG_ROOMDESC);
	SetId(GetId()..., "chassis", "body");
	SetAdjectives(GetAdjectives()..., "melting", "corroding");
	SetShort("the corroding chassis of an artificial body");
	SetSmell("The chassis emits an acrid chemical odor.");
	break;
    case 2:
	environment()->eventPrint("An acrid chemical odor fills the entire "
	  "area.", MSG_ROOMDESC);
	SetId(GetId()..., "synthetics", "pile", "pile of synthetics");
	SetShort("a pile of corroding synthetics");
	SetSmell("Its smell is nearly unbearable.");
	break;
    case 3:
	Destruct();
	return 0;
    }
    Count++;
    CallOut = call_out((: eventDecay :), DecayLife/3);
    return Count;
}









