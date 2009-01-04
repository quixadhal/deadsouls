#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_WORN_STORAGE;

static void create(){
    ::create();
    SetKeyName("combat pack");
    SetAdjectives( ({"combat", "back"}) );
    SetId( ({"pack", "backpack"}) );
    SetShort("a combat backpack");
    SetLong("A pack containing the equipment a testchar will need to jump into battle.");
    SetClosed(1);
    SetInventory(([
                "/domains/default/armor/glove_r" : 1,
                "/domains/default/armor/glove_l" : 1,
                "/domains/default/armor/pants" : 1,
                "/domains/default/weap/carving_knife" : 1,
                "/domains/default/armor/long_boot_r" : 1,
                "/domains/default/armor/collar" : 1,
                "/domains/default/armor/helmet" : 1,
                "/domains/default/armor/chainmail" : 1,
                "/domains/default/armor/long_boot_l" : 1,
                "/domains/default/armor/badge" : 1,
                ]));
    SetCanClose(1);
    SetMaxCarry(2000);
    SetMass(200);
    SetBaseCost("silver",10);
    SetDamagePoints(100);
    SetArmorType(A_CLOAK);
    SetProtection(COLD, 1);
}
void init(){
    ::init();
}
