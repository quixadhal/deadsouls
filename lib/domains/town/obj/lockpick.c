#include <lib.h>

inherit LIB_LOCKPICK;

void create(){
    lockpick::create();
    SetKeyName("lockpick");
    SetAdjectives(({"lockpicking","picking"}));
    SetId(({ "tool","pick" }));
    SetShort("a lockpick");
    SetLong("A tool for picking locks.");
    SetMass(1);
    SetBaseCost("silver", 10);
    SetPickingQuality(10);
}

void init(){
    ::init();
}
