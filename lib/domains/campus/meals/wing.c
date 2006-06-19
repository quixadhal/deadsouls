/*    /domains/Praxis/etc/ale.c
 *    from Nightmare LPMud
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
    SetMealType(MEAL_FOOD);
    SetMealMessages("You eat a chicken wing. Mmm!",
      "$N enjoys a chicken wing. It looks delicious.");
}
void eventEat(){
    this_object()->goopy();
    return;
}
int goopy(){
    tell_room(environment(this_player()),"Gppy!\n");
    return 1;
}
