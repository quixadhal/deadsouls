#include <lib.h>

inherit LIB_CHAIR;


static void create() {
    chair::create();
    SetKeyName("chair");
    SetId("chair");
    SetAdjectives( ({ "generic","nondescript" }) );
    SetShort("a generic chair");
    SetLong("A nondescript chair.");
    SetMass(150);
    SetDollarCost(15);
    SetMaxSitters(1);
}
void init(){
    ::init();
}
