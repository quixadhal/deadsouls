#include <lib.h>
#include <meal_types.h>

inherit LIB_MEAL;


static void create() {
    meal::create();
    SetKeyName("sandwich");
    SetId(({"sandwich","ham sandwich"}));
    SetShort("a ham sandwich");
    SetLong("This is a large, rather plain ham sandwich.");
    SetMass(10);
    SetStrength(20);
    SetBaseCost("silver",10);
    SetMealType(MEAL_FOOD);
    SetMealMessages("You eat a ham sandwich. Mmm!",
            "$N eats $P ham sandwich.");
}
void init(){
    ::init();
}
