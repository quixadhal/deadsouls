#include <lib.h>

inherit LIB_ITEM;

string RetStr(){
    return file_name(this_object());
}

static void create() {
    item::create();
    SetKeyName("key");
    SetId( ({ "key", "sample key" }) );
    SetAdjectives( ({ "simple","sample" }) );
    SetShort((: RetStr :));
    SetLong((: RetStr :));
    SetMass(50);
    SetBaseCost(1);
    SetDisableChance(100);
}
void init(){
    ::init();
}
