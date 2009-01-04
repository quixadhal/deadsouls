#include <lib.h>

inherit LIB_STORAGE;

void create() {
    ::create();
    SetKeyName("bag");
    SetId( ({ "bag" }) );
    SetAdjectives( ({ "small", "plastic", "a" }) );
    SetShort("a small plastic bag");
    SetLong("It is a simple plastic bag used to hold things. ");
    SetMass(5);
    SetBaseCost("silver",1);
    SetMaxCarry(50);
    SetOpacity(0);
}

void init(){
    ::init();
}
