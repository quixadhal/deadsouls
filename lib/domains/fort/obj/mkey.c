#include <lib.h>

inherit LIB_ITEM;

static void create() {
    item::create();
    SetKeyName("key");
    SetAdjectives( ({"steel", "simple"}) );
    SetId( ({"menagerie_key"}) );
    SetShort("a steel key");
    SetLong("It is a simple key with the letter M inscribed on it.");
    SetMass(50);
    SetBaseCost(1);
    SetDisableChance(100);
}
void init(){
    ::init();
}
