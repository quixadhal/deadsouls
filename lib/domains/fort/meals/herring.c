#include <lib.h>
#include <vendor_types.h>
#include <meal_types.h>
inherit LIB_MEAL;
void create(){
    ::create();
    SetKeyName("herring");
    SetId("fish","herring","tin of herring","tin");
    SetShort("a tin of herring");
    SetLong("This is a tin of herring preserved in oil.");
    SetMealType(MEAL_FOOD);
    SetMass(5);
    SetBaseCost("silver",3);
    SetAdjectives("stinky","fishy");
    SetStrength(10);
}
void init() {
    ::init();
    SetSmell(([  "default" : "Despite being preserved in oil in a container, this smells pretty darn fishy."]));
}
