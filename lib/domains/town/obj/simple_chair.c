#include <lib.h>

inherit LIB_CHAIR;

static void create() {
    chair::create();
    SetKeyName("chair");
    SetAdjectives( ({"simple", "wooden"}) );
    SetId("chair");
    SetShort("a wooden chair");
    SetLong("A simple chair, made of wood, for sitting on.");
    SetMass(150);
    SetDollarCost(15);
    SetMaxSitters(1);
}

void init(){
    ::init();
}
