#include <lib.h>
#include <meal_types.h>

inherit LIB_MEAL;


static void create()  {
    meal::create();
    SetKeyName("coffee");
    SetId( ({"cup","coffee" }) );
    SetAdjectives( ({"regular","cup of","ordinary"}) );
    SetShort("a cup of coffee");
    SetLong("An ordinary cup of coffee"); 
    SetMass(10);
    SetMealType( MEAL_DRINK | MEAL_CAFFEINE );
    SetStrength(10);
    SetBaseCost("silver",5);
    SetMealMessages("The wonderful drink is invigorating." , 
            "$N drinks $P coffee, and looks invigorated.");
}
void init(){
    ::init();
}
