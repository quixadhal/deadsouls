#include <lib.h>
#include <vendor_types.h>
inherit LIB_BOOK;

void create(){
    ::create();
    SetKeyName("Log of Shame");
    SetId( ({"shamelog", "log", "book"}) );
    SetAdjectives( ({"shame"}) );
    SetShort("The Log of Shame");
    SetLong("The Log of Shame is a book containing channel logs people should be ashamed of, and oddly, usually aren't.");
    SetSource("/domains/town/txt/shame");
    SetMass(20);
    SetBaseCost("silver",10);
    SetVendorType(VT_TREASURE);
    SetTitle("The Sample Book");
}

void init(){
    ::init();
}
