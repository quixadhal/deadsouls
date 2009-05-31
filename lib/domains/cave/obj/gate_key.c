#include <lib.h>

inherit LIB_ITEM;

static void create() {
    item::create();
    SetKeyName("key");
    SetId( ({ "key", "cavetroll key" }) );
    SetAdjectives( ({ "metal" }) );
    SetShort("a metal key");
    SetLong("It is a key made of metal.");
    SetMass(10);
    SetBaseCost("silver",1);
    SetDisableChance(100);
}

void init(){
    ::init();
}
