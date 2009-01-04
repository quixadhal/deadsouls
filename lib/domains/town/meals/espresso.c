#include <lib.h>
#include <meal_types.h>

inherit LIB_MEAL;


static void create()  {
    meal::create();
    SetKeyName("espresso");
    SetId( ({"cup","espresso" }) );
    SetAdjectives( ({"imported","cup of"}) );
    SetShort("a cup of espresso");
    SetLong("A cup of espresso"); 
    SetMass(10);
    SetMealType( MEAL_DRINK | MEAL_CAFFEINE );
    SetStrength(20);
    SetBaseCost("silver",15);
    SetMealMessages("The wonderful drink is most invigorating." , 
            "$N drinks $P espresso, and looks invigorated and a bit more wired.");
}
void init(){
    ::init();
}
