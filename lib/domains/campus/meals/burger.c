#include <lib.h>
#include <meal_types.h>
inherit LIB_MEAL;

static void create() {
    meal::create();
    SetKeyName("hamburger");
    SetId(({"hamburger","burger"}));
    SetShort("a greasy hamburger");
    SetLong("Evidently considered to be food by some people, "+
            "this little horror in a bun is greasy, flat, and slightly gray. Enjoy!");
    SetNoCondition(1);
    SetMass(15);
    SetStrength(35);
    //SetDollarCost(4);
    SetDollarCost(4);
    SetMealType(MEAL_FOOD);
    SetMealMessages("You wolf down your hamburger with gusto.",
            "$N wolfs down $P hamburger hungrily.");
}
void init(){
    ::init();
}
