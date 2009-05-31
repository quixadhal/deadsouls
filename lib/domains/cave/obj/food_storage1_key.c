#include <lib.h>

inherit LIB_ITEM;

static void create() {
    item::create();
    SetKeyName("key");
    SetId( ({ "food_storage_one","key for a door" }) );
    SetAdjectives( ({ "simple","iron","door" }) );
    SetShort("an iron key");
    SetLong("It is a simple iron key for a door.");
    SetMass(5);
    SetBaseCost("silver",1);
    SetDisableChance(1);
}

void init(){
    ::init();
}
