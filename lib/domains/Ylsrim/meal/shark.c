/*    /domains/Ylsrim/meal/shark.c
 *    from the Dead Souls Mud Library
 *    created by Descartes of Borg 960302
 */

#include <lib.h>
#include <meal_types.h>

inherit LIB_MEAL;

static void create() {
    meal::create();
    SetKeyName("shark");
    SetId( ({ "blue shark", "shark", "fish" }) );
    SetAdjectives( ({ "blue" }) );
    SetShort("a blue shark");
    SetLong("A blue shark who once looked deadly, now looks yummy.");
    SetMass(100);
    SetMealType(MEAL_FOOD);
    SetStrength(25);
    SetMealMessages("It tastes excellent.", "$N eats a blue shark.");
}
