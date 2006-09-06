#include <lib.h>
#include <meal_types.h>

inherit LIB_DUMMY;
inherit LIB_FLASK; 

static void create() {
    dummy::create();
    flask::create();
    SetKeyName("water");
    SetId(({"sea","seawater" }));
    SetAdjectives("calm","lovely");
    SetLong("A calm, lovely sea view.");
    SetShort("the sea");
    SetFlaskContents("sea water");
    SetFlaskUses(2);
    SetStrength(-15);
    SetMaxFlask(2);
    SetEverFill(1);
    SetMealType(MEAL_DRINK);
    SetNoCondition(1);
    SetInvis(1);
}
