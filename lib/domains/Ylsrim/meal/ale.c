/*    /domains/Ylsrim/meal/ale.c
 *    from the Dead Souls V Object Library
 *    created by Descartes of Borg 960302
 */

#include <lib.h>
#include <meal_types.h>

inherit LIB_MEAL;

static void create() {
    meal::create();
    SetKeyName("ale");
    SetId( ({ "bottle", "ale", "ylsrim ale" }) );
    SetShort("a bottle of ale");
    SetLong("A nice bottle of Lars' famous Ylsrim Ale.");
    SetMass(60);
    SetMealType(MEAL_DRINK | MEAL_ALCOHOL);
    SetStrength(10);
    SetMealMessages("You drink a wonderful bottle of Ylsrim Ale.",
		    "$N drinks a wonderful bottle of Ylsrim Ale.");
    SetEmptyName("bottle");
    SetEmptyShort("an empty bottle of Ylsrim Ale");
    SetEmptyLong("It is an empty brown bottle that once held some ale.");
}
