#include <lib.h>
#include <vendor_types.h>

inherit LIB_VENDOR;

static void create() {
    vendor::create();
    SetKeyName("vendor");
    SetId( ({ "shopkeep" }) );
    SetAdjectives( ({ "generic" }) );
    SetShort("a generic vendor");
    SetLevel(1);
    SetLong("A nondescript person whose job it is to sell things.");
    SetGender("male");
    SetRace("human");
    SetSkill("bargaining", 1);
    SetProperty("no bump", 1);
    SetLocalCurrency("silver");
    SetStorageRoom("/obj/room");
    SetMaxItems(100);
    SetVendorType(VT_ALL);
}
