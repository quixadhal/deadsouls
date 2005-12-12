#include <lib.h>

inherit LIB_ITEM;
inherit LIB_SIT;
inherit LIB_LIE;

inherit LIB_SURFACE;

void create(){
    ::create();
    SetMaxSitters(4);
    SetMaxLiers(2);
    SetMaxCarry(300);
    SetMass(1700);
}


