#include <lib.h>
#include <meal_types.h>

inherit LIB_MEAL;


static void create()  {
    meal::create();
    SetKeyName("milk");
    SetId("latte");
    SetShort("a cafe latte");
    SetLong("A perfect pull of espresso married to steamed milk, "
            "and,  voila!: cafe latte!");
    SetMass(10);
    SetMealType( MEAL_DRINK | MEAL_CAFFEINE );
    SetStrength(25);
    SetValue(2);
    SetMealMessages("The wonderful drink is invigorating." , 
            "$N drinks $P latte, and looks invigorated.");
}
void init(){
    ::init();
}
