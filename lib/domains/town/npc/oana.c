#include <lib.h>
#include <vendor_types.h>

inherit LIB_VENDOR;


static void create() {
    ::create();
    SetKeyName("oana");
    SetId( ({ "shopkeep","shopkeeper","keeper","vendor"}) );
    SetAdjectives( ({ "friendly","sprightly","plain","simple" }) );
    SetShort("Oana the Magic Shop keeper");
    SetLevel(12);
    SetLong("Oana is a sprightly young woman with very short "+
            "dark hair and a simple, plain "+
            "manner of dress. She is the Magic Shop "+
            "keeper, and you may trade in magical "+
            "items with her.");
    SetGender("female");
    SetMorality(40);
    SetRace("human");
    AddCurrency("silver", random(100));
    SetProperty("no bump", 1);
    SetLocalCurrency("silver");
    SetStorageRoom("/domains/town/room/magic_shop2");
    SetMaxItems(10000);
    SetVendorType(VT_MAGIC);
    SetSkill("bargaining", 1);
    SetLanguage("common", 100);
    SetDefaultLanguage("common");
}
void init(){
    ::init();
}
