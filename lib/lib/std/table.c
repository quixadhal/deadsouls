#include <lib.h>

inherit LIB_BED;

void create(){
    bed::create();
    SetShort("A table");
    SetLong("A table");
    SetMaxSitters(2);
    SetMaxLiers(1);
    SetMaxCarry(500);
    SetMass(1000);
    inventory_visible();
    inventory_accessible();
}

void init(){
    ::init();
}

int CanGet(object who){
    return bed::CanGet(who);
}

