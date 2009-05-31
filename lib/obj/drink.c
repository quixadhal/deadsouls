#include <lib.h>
#include <meal_types.h>

inherit LIB_MEAL;


static void create() {
    meal::create();
    SetKeyName("drink");
    SetId(({"drink","generic drink"}));
    SetShort("a generic drink");
    SetLong("This is a generic drink.");
    SetMass(10);
    SetStrength(20);
    SetBaseCost("silver",10);
    SetMealType(MEAL_DRINK);
    SetMealMessages("You quaff your drink. Mmm!",
            "$N quaffs $P drink.");
    SetNoCondition(1);
}
void init(){
    ::init();
}
