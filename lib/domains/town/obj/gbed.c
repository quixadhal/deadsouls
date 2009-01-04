#include <lib.h>
#include <props.h>

inherit LIB_BED;

static void create() {
    ::create();
    SetKeyName("guest bed");
    SetId( ({ "bed" }) );
    SetAdjectives( ({ "guest" }) );
    SetShort("a guest bed");
    SetLong("This is a comfortable-looking bed for guests to sleep on.");
    SetMass(1000);
    SetBaseCost("silver",800);
    SetMaxSitters(2);
    SetMaxLiers(1);
}

void init(){
    ::init();
}
