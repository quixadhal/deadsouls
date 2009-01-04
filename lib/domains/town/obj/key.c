#include <lib.h>

inherit LIB_ITEM;

static void create() {
    item::create();
    SetKeyName("key");
    SetId( ({ "key", "mansion key" }) );
    SetAdjectives( ({ "brilliant", "silver" }) );
    SetShort("a silver key");
    SetLong("It is a brilliant silver key with no markings.");
    SetMass(1);
    SetBaseCost("silver",1);
    SetDisableChance(100);
}

void init(){
    ::init();
}
