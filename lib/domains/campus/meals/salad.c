#include <lib.h>
#include <meal_types.h>
inherit LIB_MEAL;

static void create() {
    meal::create();
    SetKeyName("dish");
    SetId(({"salad","dish of salad","dish"}));
    SetShort("a little dish with a salad in it");
    SetLong("This expensive little dish consists of some wilted lettuce "+
            "and 2 orange-colored tomato slices. It is, however, not as bad "+
            "for your heart as the hamburger in the snack bar.");
    SetMass(5);
    SetMealType(MEAL_FOOD);
    SetStrength(2);
    //SetDollarCost(3);
    SetDollarCost(3);
    SetMealMessages("You finish your salad before you realize it.","$N eats "+
            "$P salad in two bites.");
    SetEmptyName("dish");
    SetEmptyShort("an empty little plastic salad dish");
    SetEmptyLong("This little dish used to contain a salad.");
}
void init(){
    ::init();
}
