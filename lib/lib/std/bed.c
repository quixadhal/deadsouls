#include <lib.h>

inherit LIB_LIE;

inherit LIB_SURFACE;

void create(){
    surface::create();

    SetMaxLiers(4);
    SetMaxLiers(2);
    SetMaxCarry(300);
    SetMass(1700);
}

    int CanGet(object who){
        if(lie::CanGet(who))
            return surface::CanGet(who);
        else return 0;
    }

