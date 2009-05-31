#include <lib.h>
#include <vendor_types.h>
inherit LIB_BOOK;

void create(){
    ::create();
    SetKeyName("builder's guidebook");
    SetId( ({"text","guidebook", "book", "guide", "builders guide", "builders guidebook"}) );
    SetAdjectives( ({"builder's", "builder", "builders", "reference"}) );
    SetShort("a Builder's Guidebook");
    SetLong("This is a reference text for Dead Souls builders.");
    SetNoCondition(1);
    SetSource("/doc/bguide");
    SetMass(20);
    SetVendorType(VT_TREASURE);
    SetTitle("Builder's Guidebook");
}
void init(){
    ::init();
}
