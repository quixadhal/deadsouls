#include <lib.h>

inherit LIB_STORAGE;

void create() {
    ::create();
    SetKeyName("bag");
    SetId( ({ "bag" }) );
    SetAdjectives( ({ "giant", "cloth", "a" }) );
    SetShort("a giant cloth bag");
    SetLong("It is a simple cloth bag used to hold things. ");
    SetMass(100);
    SetBaseCost("silver",1);
    SetMaxCarry(2000);
}

void init(){
    ::init();
}
