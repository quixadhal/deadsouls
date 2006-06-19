#include <lib.h>
#include <meal_types.h>

inherit LIB_MEAL;

static void create()  {
    meal::create();
    SetKeyName("milk");
    SetId("milk");
    SetShort("a bottle of milk");
    SetLong("A fairly small bottle of milk from some unknown creature.  It has "
      "a greyish tint to it.  It would take several of these bottles to quench "
      "that deep down bodily thirst.");
    SetMass(3);
    SetMealType(MEAL_DRINK);
    SetStrength(10);
    SetMealMessages("You gulp down the bottle of milk.", "$N gulps down a small "
      "bottle of milk.");
    SetEmptyName("bottle");
    SetEmptyShort("an empty bottle");
    SetEmptyLong("A small bottle which used to contain a portion of milk.");
}
