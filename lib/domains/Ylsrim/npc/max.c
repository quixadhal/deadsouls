/*    /domains/Ylsrim/npc/max.c
 *    From the Dead Souls V Object Library
 *    An example vendor
 *    Created by Descartes of Borg 960302
 */

#include <lib.h>
#include <vendor_types.h>

inherit LIB_VENDOR;

static void create() {
    vendor::create();
    SetKeyName("max");
    SetId("max", "vendor", "shop keeper", "keeper", "shopkeeper");
    SetShort("Max, the local armourer");
    SetLong("He buys and sells goods for armours.");
    SetLevel(15);
    SetRace( "elf");
    SetGender("male");
    SetMorality(40);
    AddCurrency("electrum", random(200));
    // Define the place where he stores stuff to sell
    SetStorageRoom("/domains/Ylsrim"+ "/room/armoury_storage");
    // How good a vendor is he? Should be 100+
    SetSkill("bargaining", 110);
    // He should stay in his shop
    SetProperty("no bump", 1);
    // He accepts electrum
    SetLocalCurrency("electrum");
    // He sells armours
    SetVendorType(VT_ARMOUR);
}
