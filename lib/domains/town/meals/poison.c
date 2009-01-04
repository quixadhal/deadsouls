#include <lib.h>
#include <meal_types.h>

inherit LIB_MEAL;
inherit LIB_MEAL_POISON;


static void create() {
    ::create();
    SetKeyName("poison");
    SetId( ({ "vial", "poison", "vial of poison" }) );
    SetAdjectives("glass","small");
    SetShort("a vial of poison");
    SetLong("A small glass vial of poison.");
    SetMass(60);
    SetMealType(MEAL_DRINK);
    SetStrength(1);
    SetMealMessages("You drink the vial of poison.",
            "$N drinks a vial of poison.");
    SetEmptyName("vial");
    SetEmptyShort("an empty vial");
    SetEmptyLong("It is an empty brown bottle that once held some liquid.");
    SetBaseCost("silver",10);
    SetPoisonStrength(70);
    SetPoisonUses(3);
}
mixed eventDrink(object who) {
    who->AddPoison(this_object()->GetPoisonStrength() * this_object()->GetPoisonUses());
    return ::eventDrink(who);
}
void init(){
    ::init();
}
