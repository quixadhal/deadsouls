#include <lib.h>
#include <meal_types.h>
#include <dirs.h>

inherit LIB_FISH;

static void create() {
    fish::create();
    SetKeyName("zyqxuwy");
    SetId( ({ "fish", "pinkfish" }) );
    SetAdjectives( ({ "pink","freshwater","fresh water" }) );
    SetShort("a pink zyqxuwy");
    SetLong("It is freshwater fish, and pink in color.");
    SetMealType(MEAL_FOOD);
    SetMass(10); 
    SetStrength(10); 
    SetBaseCost("silver", 2);
    SetFight(4);
    SetFood("/domains/town/meals/zyqxuwy");
}
void init(){
    ::init();
}
