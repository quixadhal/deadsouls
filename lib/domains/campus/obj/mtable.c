#include <lib.h>

inherit LIB_CHAIR;

void create() {
    chair::create();
    SetKeyName("wooden table");
    SetId( ({ "table" }) );
    SetAdjectives( ({ "wood","wooden", "simple", "medium-sized" "medium sized" }) );
    SetShort("a wooden table");
    SetLong("It is a simple, medium-sized table made of wood.");
    SetInventory(([
                "/domains/campus/obj/labkey" : 1,
                "/domains/campus/obj/dcell_good" : 1,
                ]));
    SetMass(274);
    SetBaseCost("silver",1);
    SetBaseCost("silver",1);
    SetMaxCarry(5000);
    inventory_visible();
    inventory_accessible();
}

void init(){
    ::init();
}
