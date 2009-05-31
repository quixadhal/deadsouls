#include <lib.h>

inherit LIB_STORAGE;

void create() {
    ::create();
    SetKeyName("crate");
    SetId( ({ "crate" }) );
    SetAdjectives( ({ "large", "wooden","wood" }) );
    SetShort("a large crate");
    SetLong("It is a wood crate used to hold things.");
    SetMass(374);
    SetMaxCarry(700);
    SetInventory( ([
                "/domains/cave/meals/jerky" : random(350)+1,
                ]) );
}

void init(){
    ::init();
}
