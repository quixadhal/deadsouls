#include <lib.h>

inherit LIB_BED;

static void create() {
    ::create();
    SetKeyName("sofa");
    SetId("sofa");
    SetAdjectives( ({ "black", "small", "tasteful", "upholstered" }) );
    SetShort("a sofa");
    SetLong("This is a small, plush sofa tastefully upholstered "+
            "in black. It appears designed for sitting on.");
    SetMass(1500);
    SetDollarCost(15);
    SetMaxSitters(2);
}

mixed CanGet(object ob) { return "The sofa does not budge.";}

void init(){
    ::init();
}
