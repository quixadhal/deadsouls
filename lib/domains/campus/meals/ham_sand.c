/*    /domains/Praxis/etc/ale.c
 *    from Dead Souls
 *    created by Descartes of Borg 950603
 */

#include <lib.h>
#include <meal_types.h>

inherit LIB_MEAL;


static void create() {
    meal::create();
    SetKeyName("sandwich");
    SetId(({"sandwich","ham sandwich"}));
    SetShort("a ham sandwich");
    SetLong("This is a rather plain ham sandwich on white bread.");
    SetMass(10);
    SetStrength(25);
    //SetDollarCost(2);
    SetDollarCost(2);
    SetMealType(MEAL_FOOD);
    SetMealMessages("You eat a ham sandwich. Mmm!",
            "$N eats $P ham sandwich.");
}
void init(){
    ::init();
}
