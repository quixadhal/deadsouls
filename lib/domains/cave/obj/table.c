#include <lib.h>

inherit LIB_TABLE;

void create() {
    ::create();
    SetKeyName("wooden table");
    SetId( ({ "table" }) );
    SetAdjectives( ({ "wood","wooden", "simple", "medium-sized" "medium sized" }) );
    SetShort("a wooden table");
    SetLong("It is a simple, medium-sized table made of wood.");
    SetInventory(([
                "/domains/cave/obj/cup" : 1,
                "/domains/cave/obj/letter" : 1,
                ]));
    SetMass(274);
    SetBaseCost("silver",1);
    SetBaseCost("silver",1);
    SetMaxCarry(500);
}

void init(){
    ::init();
}
