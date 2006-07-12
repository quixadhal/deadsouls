#include <lib.h>

inherit LIB_ITEM;

static void create() {
    item::create();
    SetKeyName("key");
    SetAdjectives( ({"normal", "iron", "medium sized", "medium"}) );
    SetId( ({"key", "gate_tower_key"}) );
    SetShort("an iron key");
    SetLong("A normal key of medium size made of iron.");
    SetMass(50);
    SetBaseCost(1);
}
void init(){
    ::init();
}
