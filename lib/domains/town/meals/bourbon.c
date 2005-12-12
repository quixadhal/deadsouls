#include <lib.h>
#include <meal_types.h>

inherit LIB_MEAL;

static void create() {
    meal::create();
    SetKeyName("ale");
    SetId( ({ "bottle", "bourbon" }) );
    SetShort("a bottle of bourbon");
    SetLong("A dark green bottle of cheap bourbon.");
    SetMass(100);
    SetBaseCost("silver",25);
    SetMealType(MEAL_DRINK | MEAL_ALCOHOL);
    SetStrength(50);
    SetMealMessages("You finish off the boubon and gasp! WHEW!! Mean stuff!",
      "$N finishes off a bottle of bourbon and gasps.");
    SetEmptyName("bottle");
    SetEmptyShort("an empty bottle of bourbon");
    SetEmptyLong("This is an empty green bottle that once held bourbon.");
}
