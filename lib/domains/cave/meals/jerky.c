#include <lib.h>
#include <meal_types.h>
inherit LIB_MEAL;

static void create() {
    meal::create();
    SetKeyName("jerky");
    SetAdjectives(({"unpalatable","dried","flattened","flat",
                "smoked","salted"}));
    SetId(({"meat","strip","strip of jerky","strips of jerky"}));
    SetShort("a strip of jerky");
    SetLong("A dried, flattened, smoked, and salted strip of "+
            "meat that looks about as durable as tree bark and less palatable.");
    SetNoCondition(1);
    SetMass(15);
    SetStrength(1);
    SetBaseCost("silver", 1);
    SetMealType(MEAL_FOOD);
    SetMealMessages("You gnaw on the jerky and manage to choke it down.",
            "$N gnaws on and chokes down $P strip of jerky.");
}

void init(){
    ::init();
}
