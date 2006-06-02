#include <lib.h>
#include <meal_types.h>
#include <dirs.h>

inherit LIB_FISH;

static void create() {
    fish::create();
    SetKeyName("herring");
    SetId( ({ "fish" }) );
    SetAdjectives( ({ "red","saltwater","salt water" }) );
    SetShort("a red herring");
    SetLong("It is saltwater fish, and red in color.");
    SetMealType(MEAL_FOOD);
    SetMass(10); 
    SetStrength(10); 
    SetBaseCost("silver", 2);
    SetFight(4);
    SetFood("/domains/town/meals/herring");
}
void init(){
    ::init();
}
