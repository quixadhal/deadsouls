#include <lib.h>
#include <meal_types.h>
inherit LIB_MEAL;

static void create() {
    meal::create();
    SetKeyName("bread");
    SetAdjectives( ({ "black","orc","orcish"}) );
    SetShort("a loaf of black bread");
    SetLong("A loaf of bread baked in a simple, inexpensive manner.");
    SetNoCondition(1);
    SetMass(50);
    SetStrength(10);
    SetBaseCost("silver", 1);
    SetMealType(MEAL_FOOD);
    SetMealMessages("You eat your black bread.",
            "$N eats $P loaf of black bread.");
}

void init(){
    ::init();
}
