#include <lib.h>

inherit LIB_STORAGE;


void create() {
    ::create();
    SetKeyName("container");
    SetId( ({ "container" }) );
    SetAdjectives( ({ "generic","nondescript" }) );
    SetShort("a generic container");
    SetLong("It is a thing that holds other things.");
    SetCanClose(1);
    SetMass(274);
    SetBaseCost("silver",1);
    SetMaxCarry(500);
}
void init(){
    ::init();
}
