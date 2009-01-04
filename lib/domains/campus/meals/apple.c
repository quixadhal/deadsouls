#include <lib.h>
#include <meal_types.h>
inherit LIB_MEAL;

static void create() {
    meal::create();
    SetKeyName("apple");
    SetId(({"fruit"}));
    SetAdjectives( ({ "fresh","tasty","tasy-looking","red"}) );
    SetShort("an apple");
    SetLong("This is a fresh, red, tasty-looking apple. ");
    SetMass(5);
    SetStrength(35);
    SetDollarCost(1);
    SetMealType(MEAL_FOOD);
    SetMealMessages("You wolf down your apple with gusto.",
            "$N wolfs down $P apple hungrily.");
}
void init(){
    ::init();
}
