#include <lib.h>
#include <damage_types.h>
inherit LIB_ITEM;

create() {
    ::create();
    SetId( ({ "dagger", "rusty dagger" }) );
    SetKeyName("dagger");
    SetShort("A rusty dagger");
    SetLong("An old dagger someone just threw out.\n");
    SetMass(350);   SetValue( 25);
    SetDamageType(KNIFE);
    SetClass(7);
    SetWeaponType("knife");
    SetWield("It's yucky, but it wields.");
}
