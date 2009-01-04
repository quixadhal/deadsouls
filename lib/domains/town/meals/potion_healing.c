#include <lib.h>
#include <vendor_types.h>
#include <meal_types.h>

inherit LIB_POTION;

static void create() {
    potion::create();
    SetKeyName("potion");
    SetId( ({ "bottle", "potion","philtrum","potion of healing","potion of minor healing" }) );
    SetAdjectives( "green", "healing","minor healing" );
    SetShort("a green potion of minor healing");
    SetLong("A bottle containing a green fluid with the magical property of restoring some health.");
    SetMass(60);
    SetMealType(MEAL_DRINK);
    SetStrength(5);
    SetMealMessages("You drink a potion.",
            "$N drinks a potion.");
    SetBaseCost("silver",500);
    SetPoints( ([ "HP" : 100 ]) );
    SetVendorType(VT_MAGIC);
}
void init(){
    ::init();
}
