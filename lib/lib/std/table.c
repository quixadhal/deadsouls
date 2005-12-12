#include <lib.h>

inherit LIB_ITEM;
inherit LIB_SIT;
inherit LIB_LIE;

inherit LIB_STORAGE;
inherit LIB_SURFACE;

void create(){
    storage::create();
    SetMaxSitters(2);
    SetMaxLiers(1);
    SetMaxCarry(500);
    SetMass(1000);
    inventory_visible();
    inventory_accessible();
}


