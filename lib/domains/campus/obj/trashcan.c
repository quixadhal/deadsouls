#include <lib.h>

inherit LIB_BASE_DUMMY;
inherit LIB_STORAGE;

static void create() {
    base_dummy::create();
    storage::create();
    SetKeyName("trashcan");
    SetId(({"can","container"}));
    SetAdjectives("trash");
    SetShort("a trashcan");
    SetLong("A container for discarding waste.");
    SetMass(1500);
    SetDollarCost(15);
    SetMaxCarry(900);
    SetInventory( ([ 
                "/domains/campus/obj/key1" : 1,
                ]) );
    SetPreventGet("It is firmly attached to the ground.");
}
void init(){
    ::init();
}
