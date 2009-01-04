#include <lib.h>
#include <meal_types.h>

inherit LIB_MEAL;


static void create() {
    meal::create();
    SetKeyName("meal");
    SetId(({"meal","generic meal"}));
    SetShort("a generic meal");
    SetLong("This is a generic meal.");
    SetMass(10);
    SetStrength(20);
    SetBaseCost("silver",10);
    SetMealType(MEAL_FOOD);
    SetMealMessages("You eat your food. Mmm!",
            "$N eats $P food.");
    SetNoCondition(1);
}
void init(){
    ::init();
}
