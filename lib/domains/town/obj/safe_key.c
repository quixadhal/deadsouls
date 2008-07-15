#include <lib.h>

inherit LIB_ITEM;

static void create() {
    item::create();
    SetKeyName("safe key");
    SetId( ({ "key", "mansion safe key" }) );
    SetAdjectives( ({ "complex" }) );
    SetShort("a complex key");
    SetLong("It is a complicated-looking key.");
    SetMass(10);
    SetBaseCost("silver",1);
    SetDisableChance(100);
}
void init(){
    ::init();
}
