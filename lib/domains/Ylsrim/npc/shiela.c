/*    /domains/Ylsrim/npc/shiela.c
 *    From the Dead Souls Mud Library
 *    An example vendor.
 *    Created by Descartes of Borg 960302
 */

#include <lib.h>
#include <vendor_types.h> // defines VT_WEAPON

inherit LIB_VENDOR;

static void create() {
    vendor::create();
    SetKeyName("shiela");
    SetId("shiela", "vendor", "shop keeper", "keeper", "shopkeeper");
    SetShort("Shiela, the local weapon vendor");
    SetLong("She buys and sells weapons.");
    SetLevel(15);
    SetRace( "human");
    SetGender("female");
    SetMorality(40);
    SetListen("She has a very gruff voice.");
    AddCurrency("electrum", random(200));
    // the room where she stores stuff to sell
    SetStorageRoom("/domains/Ylsrim"+ "/room/weaponry_storage");
    // How good is she at being a vendor? Should be 100+
    SetSkill("bargaining", 150);
    // This vendor belongs in a particular shop, don't bump her
    SetProperty("no bump", 1);
    // She takes electrum coins
    SetLocalCurrency("electrum");
    // She buys weapons
    SetVendorType(VT_WEAPON);
}

