/*    /domains/Praxis/etc/ale.c
 *    from Dead Souls
 *    created by Descartes of Borg 950603
 */

#include <lib.h>
#include <meal_types.h>

inherit LIB_MEAL;


static void create() {
    meal::create();
    SetKeyName("ale");
    SetId( ({ "bottle", "ale", "praxis ale" }) );
    SetShort("a bottle of ale");
    SetLong("A nice bottle of Lars' famous Praxis Ale.");
    SetMass(60);
    SetMealType(MEAL_DRINK | MEAL_ALCOHOL);
    SetStrength(20);
    SetMealMessages("You drink a wonderful bottle of Praxis Ale.",
            "$N drinks a wonderful bottle of Praxis Ale.");
    SetEmptyName("bottle");
    SetEmptyShort("an empty bottle of Praxis Ale");
    SetEmptyLong("It is an empty brown bottle that once held some ale.");
}
void init(){
    ::init();
}
