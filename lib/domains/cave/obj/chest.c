#include <lib.h>

inherit LIB_STORAGE;

void create() {
    ::create();
    SetKeyName("chest");
    SetAdjectives(({"wooden","heavy"}));
    SetId(({"chest"}));
    SetShort("a wooden chest");
    SetLong("This is a heavy wooden chest used to store items.");
    SetInventory(([
                "/domains/cave/obj/letter2" : 1,
                "/domains/cave/armor/pajamas" : 1,
                "/domains/cave/obj/food_storage1_key" : 1,
                "/domains/cave/obj/letter3" : 1,
                ]));
    SetMass(2000);
    SetBaseCost("silver",50);
    SetMaxCarry(500);
    SetCanClose(1);
    SetClosed(1);
}

void init(){
    ::init();
}
