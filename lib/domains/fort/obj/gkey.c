#include <lib.h>

inherit LIB_ITEM;

static void create() {
    item::create();
    SetKeyName("key");
    SetId( ({ "grand_gate_key" }) );
    SetAdjectives( ({ "iron","heavy" }) );
    SetShort("a large iron key");
    SetLong("It is a large, heavy key made of iron, about the size of a human hand.");
    SetMass(50);
    SetBaseCost(1);
    //SetDisableChance(100);
}

