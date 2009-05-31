#include <lib.h>
#include <vendor_types.h>
#include <meal_types.h>

inherit LIB_POTION;

static void create() {
    potion::create();
    SetKeyName("potion");
    SetId( ({ "bottle", "potion","philtrum","potion of strength" }) );
    SetAdjectives( "red", "strength", "strongitude", "strongicity" );
    SetShort("a red potion of strength");
    SetLong("A bottle containing a red fluid with the magical property of making one stronger.");
    SetMass(60);
    SetMealType(MEAL_DRINK);
    SetStrength(5);
    SetMealMessages("You drink a potion.",
            "$N drinks a potion.");
    SetBaseCost("silver",1000);
    SetDuration(300);
    SetStats( ([ "strength" : 10 ]) );
    SetPoints( ([ "HP" : 50, "SP" : 50 ]) );
    SetVendorType(VT_MAGIC);
}
void init(){
    ::init();
}
