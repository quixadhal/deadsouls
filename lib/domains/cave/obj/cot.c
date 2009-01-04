#include <lib.h>
#include <props.h>

inherit LIB_BED;

static void create() {
    ::create();
    SetKeyName("cot");
    SetId( ({ "cot","bed" }) );
    SetAdjectives( ({ "simple"}) );
    SetShort("a simple cot");
    SetLong("This is a simple cot designed for "
            "sleepers that do not require a comfortable bed.");
    SetMass(500);
    SetBaseCost("silver",15);
    SetMaxSitters(2);
    SetMaxLiers(1);
}

void init(){
    ::init();
}
