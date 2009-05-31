/*    /domains/Praxis/npc/otik.c
 *    from Dead Souls
 *    created by Descartes of Borg 950603
 */

#include <lib.h>
#include <vendor_types.h>

inherit LIB_VENDOR;

static void create() {
    ::create();
    SetKeyName("yulia");
    SetId( ({ "vendor","secretary","assistant","manager" }) );
    SetAdjectives( ({ "executive","medical","guild" }) );
    SetShort("Yulia, the clinic assistant");
    SetLevel(12);
    SetLong("Yulia is typical of office assistants: she is a bit "+
            "arrogant, officious, and brusque; but she appears "+
            "to be a competent enough secretary. "+
            "She is the person you will buy a treatment slip "+
            "from. Read the list on the wall for the slips "+
            "available.");
    SetGender("female");
    SetMorality(40);
    SetRace("human");
    AddCurrency("dollars", random(100));
    SetProperty("no bump", 1);
    SetLocalCurrency("dollars");
    SetStorageRoom("/domains/campus/room/healer2");
    SetMaxItems(10000);
    SetVendorType(VT_HERB);
    SetSkill("bargaining", 1);

}
