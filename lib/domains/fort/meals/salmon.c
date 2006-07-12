#include <lib.h>
#include <vendor_types.h>
#include <meal_types.h>
inherit LIB_MEAL;
void create(){
    ::create();
    SetKeyName("salmon");
    SetId("salmon","fish","slab");
    SetShort("a thick slab of salmon");
    SetLong("This is a fatty, thick slab of salmon. It appears fresh and delicious.");
    SetMealType(MEAL_FOOD);
    SetMass(10);
    SetBaseCost("silver",15);
    SetAdjectives("fatty","thick","slab of");
    SetStrength(20);
}
void init() {
    ::init();
    SetSmell(([  "default" : "For a fish, it is remarkably free of any odor."]));
}
