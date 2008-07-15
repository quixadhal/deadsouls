#include <lib.h>
#include <damage_types.h>
#include <vendor_types.h>
#include <boobytraps.h>

inherit LIB_ITEM;
int AlreadyTrapped = 0;

void boobytrap_me(){
    object shadowtrap;
    if(!clonep(this_object())) return; /* Avoids boobytrapping the blueprint ob */
    if(AlreadyTrapped) return;

    AlreadyTrapped = 1;
    shadowtrap = new("/shadows/needle_trap");
    shadowtrap->SetTrapDescription("A poison needle trap.");
    shadowtrap->SetTrapLevel(50);
    shadowtrap->SetAutoResets(1);
    shadowtrap->SetTrapType(BOOBYTRAP_WIELD);
    shadowtrap->eventShadow(this_object());
}

static void create() {
    item::create();
    AddSave(({ "AlreadyTrapped" }) ); /* Shouldn't re-arm when you relog */
    SetKeyName("sharp dagger");
    SetId( ({ "dagger"}));
    SetAdjectives( ({ "sharp","fine","wicked","wicked-looking"}));
    SetShort("a wicked-looking dagger");
    SetLong("A sharp, wicked dagger.");
    SetMass(150);
    SetBaseCost("silver",50);
    SetVendorType(VT_WEAPON);
    SetClass(10);
    SetDamageType(KNIFE);
    SetWeaponType("knife");
    boobytrap_me();
}

void init(){
    ::init();
}
