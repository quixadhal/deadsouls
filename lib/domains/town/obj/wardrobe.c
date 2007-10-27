#include <lib.h>

inherit LIB_STORAGE;

void create() {
    ::create();
    SetKeyName("wardrobe");
    SetId(({"wardrobe"}));
    SetAdjectives(({"wood","tall"}));
    SetShort("a tall wooden wardrobe");
    SetLong("This piece of furniture is designed to hold "+
      "clothing and related accessories.");
    SetInventory(([
        "/domains/town/armor/long_boot_l" : 1,
        "/domains/town/armor/bearsuit" : 1,
        "/domains/town/armor/scarf" : 1,
        "/domains/town/armor/belt" : 1,
        "/domains/town/armor/goggles" : 1,
        "/domains/town/armor/shirt" : 1,
        "/domains/town/armor/jeans" : 1,
        "/domains/town/armor/hunting_cap" : 1,
        "/domains/town/armor/vest" : 1,
        "/domains/town/armor/long_boot_r" : 1,
        "/domains/town/armor/jacket" : 1,
        "/domains/town/armor/overcoat" : 1,
      ]));
    SetMass(2000);
    SetBaseCost("silver",150);
    SetMaxCarry(1000);
    SetCanClose(1);
    SetClosed(1);
    AddMoney("silver",100);
}
void init(){
    ::init();
}
