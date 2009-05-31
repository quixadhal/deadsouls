#include <lib.h>
#include <meal_types.h>
inherit LIB_MEAL;

static void create() {
    meal::create();
    SetKeyName("bad apple");
    SetId(({"fruit","apple"}));
    SetAdjectives( ({ "foul","yucky","rotten-looking","browned","rotten"}) );
    SetShort("a rotten apple");
    SetLong("This is a foul, browned, rotten-looking apple. ");
    SetMass(5);
    SetStrength(-5);
    SetDollarCost(1);
    SetMealType(MEAL_FOOD);
    SetMealMessages("You eat your rotten apple. Gross!",
            "$N eats $P rotten apple. Gross!");
}
void init(){
    ::init();
}
