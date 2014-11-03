#include <lib.h>
#include <vendor_types.h>
#include <meal_types.h>

inherit LIB_POTION;

static void create() {
    potion::create();
    SetKeyName("eve tonic");
    SetId( ({ "bottle", "liquid", "eve", "tonic"}) );
    SetAdjectives( "eve", "glowing","liquid", "blue" );
    SetShort("a bottle of Eve");
    SetLong("A bottle of glowing blue liquid. It has a label on it featuring "+
            "the silhouette of a bearded man wearing a top hat.");
    SetEmptyLong("An empty bottle. It has a label on it featuring "+
            "the silhouette of a bearded man wearing a top hat."); 
    SetMass(60);
    SetMealType(MEAL_DRINK);
    SetStrength(5);
    SetMealMessages("You drink a potion.",
            "$N drinks a potion.");
    SetBaseCost("silver",500);
    SetPoints( ([ "MP" : 500 ]) );
    SetVendorType(VT_MAGIC);
    SetItems(([
                ({"label"}) : "A label affixed to the bottle.",
                ]));
    SetRead( ({"label"}),  "Invigorating EVE! Stay awake!" );
}
void init(){
    ::init();
}
