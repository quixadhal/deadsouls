#include <lib.h>
#include <meal_types.h>
#include <jump.h>

inherit LIB_BASE_DUMMY;
inherit LIB_FLASK; 
inherit LIB_JUMP; 

varargs int eventJump(mixed args...) {
    return ::eventJump(args...);
}

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
    foreach(mixed ident in GetId()){
        AddJump(ident,"/domains/town/virtual/surface/33,100000",JUMP_INTO);
    }
}

mixed CanGet(object ob) { return "#The sea stays in place.";}
