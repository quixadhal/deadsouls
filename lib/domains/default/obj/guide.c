#include <lib.h>
#include <vendor_types.h>
inherit LIB_BOOK;

void create(){
    ::create();
    SetKeyName("administrator's guidebook");
    SetId( ({"guidebook", "book", "guide", "administrators guide", "administrators guidebook"}) );
    SetAdjectives( ({"admin's", "admin", "admins", "administrator", "administrators", "reference"}) );
    SetShort("an Administrator's Guidebook");
    SetLong("This is a reference text for Dead Souls administrators.");
    SetNoCondition(1);
    SetSource("/doc/guide");
    SetMass(20);
    SetBaseCost("silver",10);
    SetVendorType(VT_TREASURE);
    SetTitle("Administrator's Guidebook");
}
void init(){
    ::init();
}
