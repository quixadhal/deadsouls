#include <lib.h>

inherit LIB_STORAGE;

void create() {
    ::create();
    SetKeyName("bag");
    SetAdjectives( ({"small", "cloth"}) );
    SetId( ({ "bag" }) );
    SetShort("a small cloth bag");
    SetLong("It is a simple cloth bag used to hold things. ");
    SetMass(5);
    SetBaseCost("silver",1);
    SetMaxCarry(50);
}
void init(){
    ::init();
}
