#include <lib.h>
#include <props.h>

inherit LIB_BED;


static void create() {
    ::create();
    SetKeyName("bed");
    SetId( ({ "bed" }) );
    SetAdjectives( ({ "generic","nondescript"}) );
    SetShort("a generic bed");
    SetLong("A nondescript bed.");
    SetMass(400);
    SetBaseCost("silver",150);
    SetMaxSitters(2);
    SetMaxLiers(1);
}
void init(){
    ::init();
}
