#include <lib.h>

inherit LIB_STORAGE;

void create() {
    ::create();
    SetKeyName("box");
    SetId( ({ "box" }) );
    SetAdjectives( ({ "wooden","wood" }) );
    SetShort("a wooden box");
    SetLong("It is a wooden box used to hold things.");
    SetMass(274);
    SetMaxCarry(500);
    SetInventory( ([
                "/domains/cave/meals/jerky" : random(100)+1,
                ]) );
}

void init(){
    ::init();
}
