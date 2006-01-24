#include <lib.h>
#include <vendor_types.h>
inherit LIB_BOOK;

void create(){
    ::create();
    SetKeyName("generic book");
    SetId( ({"book"}) );
    SetAdjectives( ({"generic","sample","template"}) );
    SetShort("a generic book");
    SetLong("This is a mostly empty book.");
    SetMass(20);
    SetBaseCost("silver",10);
    SetVendorType(VT_TREASURE);
    SetTitle("The Sample Book");
}
void init(){
    ::init();
}
