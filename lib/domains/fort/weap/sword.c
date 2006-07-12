#include <lib.h>
#include <damage_types.h>
#include <vendor_types.h>

inherit LIB_ITEM;

static void create() {
    item::create();
    SetKeyName("plain sword");
    SetId( ({ "sword", "plain sword" }) );
    SetAdjectives( ({ "plain" }) );
    SetShort("a plain sword");
    SetLong("A cheap and rather dull plain sword.");
    SetProperties(([
	"alpha" : 1,
      ]));
    SetMass(150);
    SetHands(1);
    SetDollarCost(50);
    SetVendorType(VT_WEAPON);
    SetClass(20);
    SetDamageType(BLADE);
    SetWeaponType("blade");
}
void init(){
    ::init();
}
