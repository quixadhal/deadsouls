/*    /domains/Praxis/etc/ale.c
 *    from Dead Souls
 *    created by Descartes of Borg 950603
 */

#include <lib.h>
#include <meal_types.h>

inherit LIB_MEAL;

static void create() {
    meal::create();
    SetKeyName("gatorade");
    SetId(({"gatorade","drink","sports drink","bottle"}));
    SetShort("a bottle of gatorade");
    SetLong("This is a bottle of lemon-lime Gatorade. ");
    SetMass(60);
    SetBaseCost("silver", 2);
    SetMealType(MEAL_DRINK);
    SetStrength(40);
    SetMealMessages("You quaff the bottle of gatorade, and feel new "+
            "and refreshed. Aaah!","$N gulps down $P gatorage and sighs in "+
            "contentment.");
    SetEmptyName("bottle");
    SetEmptyShort("an empty bottle of gatorade");
    SetEmptyLong("This is an empty bottle that once held that tasty "+
            "beverage and sports drink, Gatorade.");
}
void init(){
    ::init();
}
