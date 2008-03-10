#include <lib.h>
#include <meal_types.h>

inherit LIB_BASE_DUMMY;
inherit LIB_FLASK; 

static void create() {
    base_dummy::create();
    flask::create();
    SetKeyName("seawater");
    SetId(({"sea","sea water","ocean","water" }));
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
    SetProperty("buoyant",1);
}

mixed CanGet(object ob) { return "#The sea stays in place.";}
