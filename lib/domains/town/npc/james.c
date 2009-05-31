#include <lib.h>
#include <vendor_types.h>

inherit LIB_VENDOR;

static void create() {
    ::create();
    SetKeyName("james");
    SetId( ({ "vendor","secretary","assistant","manager" }) );
    SetAdjectives( ({ "executive","medical","guild" }) );
    SetShort("James, the guild secretary and manager");
    SetLevel(12);
    SetLong("James is typical of apprentice healers: he is a bit "+
            "arrogant, officious, and brusque; but he appears "+
            "to be a competent secretary and guild officer.  "+
            "He is the person you will buy a treatment token "+
            "from. Read the list on the wall for the tokens "+
            "available.");
    SetGender("male");
    SetMorality(40);
    SetRace("human");
    AddCurrency("silver", random(100));
    SetSkill("bargaining", 1);
    SetProperty("no bump", 1);
    SetLocalCurrency("silver");
    SetStorageRoom("/domains/town/room/healer2");
    SetMaxItems(10000);
    SetVendorType(VT_HERB);
    SetSkill("bargaining", 1);
    SetPolyglot(1);
    SetLanguage("common", 100);
    SetDefaultLanguage("common");
}

void init(){
    ::init();
}
