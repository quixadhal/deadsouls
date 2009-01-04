#include <lib.h>
#include <vendor_types.h>
inherit LIB_DIE;


void create(){
    ::create();
    SetKeyName("die");
    SetId( ({ "implement"}) );
    SetAdjectives( ({"20","twenty","sided","twenty-sided","20-sided"}) );
    SetShort("a twenty-sided die");
    SetLong("A typical implement of games of chance. Orange in color, and "
            "marked with numbers rather than dots along its twenty surfaces.");
    SetDenominator(20);
    SetMass(4);
    SetBaseCost("silver",5);
    SetVendorType(VT_TREASURE);
}

void init(){
    ::init();
}
