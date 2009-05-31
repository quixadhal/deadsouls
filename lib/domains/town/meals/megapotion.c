#include <lib.h>
#include <meal_types.h>

inherit LIB_POTION;

static void create() {
    potion::create();
    SetKeyName("potion");
    SetId( ({ "potion","philtrum","megapotion","megapotion of uberness" }) );
    SetShort("a megapotion of uberness");
    SetLong("An example of a potion that is totally out of proportion in power.");
    SetMass(60);
    SetMealType(MEAL_DRINK | MEAL_ALCOHOL);
    SetStrength(5);
    SetMealMessages("You drink a potion.",
            "$N drinks a potion.");
    SetBaseCost("silver",10000);
    SetDuration(30);
    SetStats( ([ "strength" : 50, "speed" : 50 ]) );
    SetSkills( ([ "bargaining" : 20, "melee defense" : 20 ]) );
    SetPoints( ([ "HP" : 100, "XP" : 100, "SP" : 100, "MP" : 100 ]) );
}
void init(){
    ::init();
}
