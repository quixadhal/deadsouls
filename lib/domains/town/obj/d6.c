#include <lib.h>
#include <vendor_types.h>
inherit LIB_DIE;


void create(){
    ::create();
    SetKeyName("die");
    SetId( ({ "implement"}) );
    SetAdjectives( ({"6","six","sided","six-sided","6-sided"}) );
    SetShort("a six-sided die");
    SetLong("A typical implement of games of chance. Orange in color, and "
            "marked with numbers rather than dots along its six surfaces.");
    SetMass(2);
    SetBaseCost("silver",1);
    SetVendorType(VT_TREASURE);
}

void init(){
    ::init();
}
