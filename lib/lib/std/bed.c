#include <lib.h>

inherit LIB_ITEM;
inherit LIB_LIE;

inherit LIB_SURFACE;

void create(){
    item::create();
    surface::create();

    SetMaxLiers(4);
    SetMaxLiers(2);
    SetMaxCarry(300);
    SetMass(1700);
}

int CanGet(object who){
    if(lie::CanGet(who))
	return item::CanGet(who);
    else return 0;
}

