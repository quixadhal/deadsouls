#include <lib.h>

inherit LIB_ITEM;


static void create() {
    item::create();
    SetKeyName("key");
    SetId( ({ "key", "sample key" }) );
    SetAdjectives( ({ "simple","sample" }) );
    SetShort("a sample key");
    SetLong("It is a simple sample key with no markings.");
    SetMass(50);
    SetBaseCost(1);
    SetDisableChance(100);
}
void init(){
    ::init();
}
