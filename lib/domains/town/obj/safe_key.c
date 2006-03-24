#include <lib.h>

inherit LIB_ITEM;


static void create() {
    item::create();
    SetKeyName("key");
    SetId( ({ "key", "mansion safe key" }) );
    SetAdjectives( ({ "small" }) );
    SetShort("a small key");
    SetLong("It is a small key.");
    SetMass(10);
    SetBaseCost("silver",1);
    SetDisableChance(100);
}
void init(){
    ::init();
}
