/*    /domains/Ylsrim/etc/pole.c
 *    from the Dead Souls Mud Library
 *    created by Descartes of Borg 951009
 */

#include <lib.h>
#include <vendor_types.h>

inherit LIB_POLE;
inherit LIB_ITEM;

static void create() {
    pole::create();
    item::create();
    SetKeyName("bamboo pole");
    SetId( ({ "pole", "bamboo pole", "stick"  }) );
    SetAdjectives( ({ "bamboo", "feeble" }) );
    SetShort("a bamboo pole");
    SetLong("It is a feeble bamboo stick with a string at the end of it.");
    SetClass(1);
    SetValue(10);
    SetMass(30);
    SetVendorType(VT_TREASURE | VT_FISHING);
    SetStrength(200);
    SetChance(20);
}

static mixed AddSave(mixed *var) {
    return item::AddSave(var);
}
