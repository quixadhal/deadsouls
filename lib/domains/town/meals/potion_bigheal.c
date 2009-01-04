#include <lib.h>
#include <vendor_types.h>
#include <meal_types.h>

inherit LIB_POTION;

static void create() {
    potion::create();
    SetKeyName("potion");
    SetId( ({ "bottle", "potion","philtrum","potion of healing","potion of greater healing" }) );
    SetAdjectives(({"green", "healing","greater healing"}));
    SetShort("a green potion of greater healing");
    SetLong("A bottle containing a green fluid with the magical property of restoring much health.");
    SetMass(60);
    SetMealType(MEAL_DRINK);
    SetStrength(15);
    SetMealMessages("You drink a potion.",
            "$N drinks a potion.");
    SetBaseCost("silver",5500);
    SetPoints( ([ "HP" : 1000 ]) );
    SetVendorType(VT_MAGIC);
}
void init(){
    ::init();
}
