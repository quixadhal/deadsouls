#include <lib.h>
#include <vendor_types.h>
#include <meal_types.h>
inherit LIB_MEAL;
void create(){
    ::create();
    SetKeyName("tuna");
    SetId("tuna","fish","slab");
    SetShort("a thick slab of tuna");
    SetLong("This is a fatty, thick slab of tuna fish. It appears fresh and delicious.");
    SetMealType(MEAL_FOOD);
    SetMass(10);
    SetBaseCost("silver",10);
    SetAdjectives("fatty","thick","slab of");
    SetStrength(10);
}
void init() {
    ::init();
    SetSmell(([  "default" : "For a fish, it is remarkably free of any odor."]));
}
