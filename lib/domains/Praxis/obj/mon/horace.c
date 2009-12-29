#include <vendor_types.h>
inherit "/lib/std/vendor";

create() {
    ::create();
    SetKeyName("horace");
    SetId( ({ "horace", "vendor", "shop keeper", "keeper", "shopkeeper", "nerd" }) );
    SetShort("Horace, the shop keeper of the adventurer's supply shop");
    SetAggressive( 0);
    SetLevel(12);
    SetLong("He buys and sells goods for adventuring.\n"+
            "<help shop> will get you a list of shop commands.\n");
    SetNativeLanguage( "eltherian" );
    SetGender("male");
    SetMorality(40);
    SetRace( "elf");
    SetHealthPoints(480);
    AddCurrency("electrum", random(200));
    SetRace("human");
    SetLocalCurrency("electrum");
    SetVendorType(VT_TREASURE);
    SetStorageRoom("/domains/Praxis/storage");
    SetSkill("melee",100);
    SetSkill("bargaining", 150);
    SetProperty("no bump", 1);
}

int is_invincible() {
    SetStaminaPoints(GetMaxStaminaPoints());
    this_object()->eventForce("shout in eltherian Help!! I am being attacked by "+
            previous_object()->query_cap_name());
    return 0;
}
