#include <lib.h>

inherit LIB_STORAGE;

void create() {
    ::create();
    SetKeyName("barrel");
    SetId( ({ "barrel" }) );
    SetAdjectives( ({ "simple","oak","oaken","wood","wooden" }) );
    SetShort("an oak barrel");
    SetLong("It is a simple wooden barrel used for holding things.");
    SetMass(474);
    SetMaxCarry(500);
    SetInventory( ([
                "/domains/cave/meals/herring" : random(100),
                ]) );
}

void init(){
    ::init();
}
