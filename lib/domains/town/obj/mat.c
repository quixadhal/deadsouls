#include <lib.h>
#include <vendor_types.h>
inherit LIB_ITEM;

void create(){
    ::create();
    SetKeyName("bath mat");
    SetId( ({"mat"}) );
    SetAdjectives( ({"slightly damp","damp","blue","light blue"}) );
    SetShort("a bath mat");
    SetLong("This is a mat used to help prevent slipping, promote "
            "foot drying, and avoid a big wet mess on "
            "the bathroom floor. It is slightly damp.");
    SetMass(10);
    SetBaseCost("silver",5);
    SetVendorType(VT_TREASURE);
}

void init(){
    ::init();
}
