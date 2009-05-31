#include <lib.h>
#include <meal_types.h>

inherit LIB_MEAL;


static void create()  {
    meal::create();
    SetKeyName("water");
    SetId(({ "bottle","water"}));
    SetAdjectives(({"fresh","spring"}));
    SetShort("a bottle of water");
    SetLong("This is a little bottle of fresh spring water.");
    SetMass(10);
    SetMealType(MEAL_DRINK);
    SetStrength(10);
    SetBaseCost("silver",2);
    SetMealMessages("You drink your water, and feel refreshed.", "$N drinks "+
            "$P water.");
}
void init(){
    ::init();
}
