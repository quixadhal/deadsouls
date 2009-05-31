#include <lib.h>
#include <meal_types.h>
inherit LIB_MEAL;

static void create() {
    meal::create();
    SetKeyName("roast beast");
    SetId( ({ "meat","beast","roast","hunk"}) );
    SetAdjectives( ({ "savory","hunk of","spiced","delicious"}) );
    SetShort("a hunk of roast beast");
    SetLong("Savory and spiced, this hunk of meat looks delicious.");
    SetNoCondition(1);
    SetMass(50);
    SetStrength(15);
    SetBaseCost("silver", 20);
    SetMealType(MEAL_FOOD);
    SetMealMessages("You eat your meat.",
            "$N eats $P hunk of roast beast.");
}

void init(){
    ::init();
}
