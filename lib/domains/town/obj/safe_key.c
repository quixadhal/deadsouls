#include <lib.h>

inherit LIB_ITEM;

static void create() {
    item::create();
    SetKeyName("complex key");
    SetId( ({ "key", "mansion safe key" }) );
    SetAdjectives( ({ "complex", "complicated", "complicated looking" }) );
    SetShort("a complex key");
    SetLong("It is a complicated-looking key.");
    SetMass(1);
    SetBaseCost("silver",1);
    SetDisableChance(100);
}

void init(){
    ::init();
}
