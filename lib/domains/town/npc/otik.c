#include <lib.h>
#include <vendor_types.h>

inherit LIB_VENDOR;

static void create() {
    ::create();
    SetKeyName("otik");
    SetId( ({ "vendor","otik", "owner", "shopkeep", "shopkeeper", "keeper" }) );
    SetShort("Otik, the keeper of the shop");
    SetLevel(12);
    SetLong("Otik is the owner of the local general store, an old and famous "
            "trading post in the heart of town.  The brother of Lars, he also is an "
            "unassuming fellow, and just as jovial.");
    SetGender("male");
    SetMorality(40);
    SetRace("human");
    AddCurrency("electrum", random(200));
    SetSkill("bargaining", 1);
    SetProperty("no bump", 1);
    SetLocalCurrency("silver");
    SetStorageRoom("/domains/town/room/shop2");
    SetMaxItems(10000);
    SetVendorType(VT_ALL);
    SetAttackable(0);
    SetLanguage("common", 100);
    SetDefaultLanguage("common");
}
void init(){
    ::init();
}
