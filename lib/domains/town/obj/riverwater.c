#include <lib.h>
#include <meal_types.h>

inherit LIB_DUMMY;
inherit LIB_FLASK; 

static void create() {
    dummy::create();
    flask::create();
    SetKeyName("water");
    SetId(({"river","riverwater" }));
    SetAdjectives("powerful","river");
    SetLong("The edge of a powerful river.");
    SetShort("the edge of a powerful river");
    SetFlaskContents("water");
    SetFlaskUses(2);
    SetStrength(5);
    SetMaxFlask(2);
    SetEverFill(1);
    SetMealType(MEAL_DRINK);
    SetNoCondition(1);
    SetInvis(1);
}
