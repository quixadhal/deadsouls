#include <lib.h>
#include <meal_types.h>

inherit LIB_MEAL;

static void create()  {
    meal::create();
    SetKeyName("milk");
    SetId("milk");
    SetAdjectives(({"milk1","milk2"}));
    SetShort("a carton of milk");
    SetLong("This is a little carton of grade-A milk.");
    SetMass(10);
    SetBaseCost("silver", 1);
    SetMealType(MEAL_DRINK);
    SetStrength(25);
    SetMealMessages("You drink your milk, and feel refreshed.", "$N drinks "+
      "$P milk.");
    SetEmptyName("carton");
    SetEmptyShort("an empty carton of milk");
    SetEmptyLong("A little carton container that once held "+
      "grade-A milk.");
}
void init(){
    ::init();
}
