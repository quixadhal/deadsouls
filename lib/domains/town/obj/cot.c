#include <lib.h>
#include <props.h>

inherit LIB_BED;
inherit LIB_SMELL;


static void create() {
    ::create();
    SetKeyName("metal cot");
    SetId( ({ "cot","bed" }) );
    SetAdjectives( ({ "simple","metal" }) );
    SetShort("a simple cot");
    SetLong("This is a simple metal cot designed for "
            "sleepers that do not require a comfortable bed.");
    SetMass(50);
    SetBaseCost("silver",15);
    SetMaxSitters(2);
    SetMaxLiers(1);
}

mixed CanGet(object ob) { return "The cot is bolted to the floor.";}

void init(){
    ::init();
}
