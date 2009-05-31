#include <lib.h>
#include <climb.h> // defines CLIMB_DOWN

inherit LIB_ITEM;
inherit LIB_CLIMB;

static void create() {
    ::create();
    SetKeyName("ladder");
    SetId("ladder");
    SetAdjectives(({"short","wood","wooden"}));
    SetShort("a ladder");
    SetLong("This is a standard wooden ladder, suitable for climbing.");
    SetMass(100);
    SetBaseCost("silver",10);
}

void init(){
    ::init();
}
