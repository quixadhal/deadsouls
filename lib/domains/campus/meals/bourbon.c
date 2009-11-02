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
    SetId( ({ "bottle", "bourbon" }) );
    SetShort("a bottle of bourbon");
    SetLong("A dark green bottle of cheap bourbon.");
    SetMass(100);
    SetMealType(MEAL_DRINK | MEAL_ALCOHOL);
    SetStrength(100);
    SetMealMessages("You finish off the bourbon and gasp! WHEW!! Mean stuff!",
            "$N finishes off a bottle of bourbon and lets loose a rebel yell!");
    SetEmptyName("bottle");
    SetEmptyShort("an empty bottle of bourbon");
    SetEmptyLong("This is an empty green bottle that once held bourbon.");
}
void init(){
    ::init();
}
