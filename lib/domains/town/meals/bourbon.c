#include <lib.h>
#include <meal_types.h>

inherit LIB_MEAL;


static void create() {
    meal::create();
    SetKeyName("bourbon");
    SetAdjectives( ({ "dark","green","cheap","bourbon" }) );
    SetId( ({ "bottle", "bottle of bourbon" }) );
    SetShort("a bottle of bourbon");
    SetLong("A dark green bottle of cheap bourbon.");
    SetMass(100);
    SetBaseCost("silver",20);
    SetMealType(MEAL_DRINK | MEAL_ALCOHOL);
    SetStrength(30);
    SetMealMessages("You finish off the bourbon and gasp! WHEW!! Mean stuff!",
            "$N finishes off a bottle of bourbon and gasps.");
}
void init(){
    ::init();
}
