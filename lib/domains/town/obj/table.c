#include <lib.h>

inherit LIB_STORAGE;
inherit LIB_CHAIR;
inherit "/lib/comp/surface";

void create() {
    ::create();
    SetKeyName("wooden table");
    SetId( ({ "table" }) );
    SetAdjectives( ({ "wood","wooden", "simple", "medium-sized" "medium sized" }) );
    SetShort("a wooden table");
    SetLong("It is a simple, medium-sized table made of wood.");
    SetInventory(([
	"/domains/default/obj/handbook" : 1,
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
