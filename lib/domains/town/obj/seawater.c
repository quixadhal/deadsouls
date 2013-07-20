#include <lib.h>
#include <meal_types.h>
#include <jump.h>

inherit LIB_BASE_DUMMY;
inherit LIB_FLASK; 
inherit LIB_JUMP; 
inherit LIB_EXITS; 
inherit LIB_ENTER; 

varargs int eventJump(mixed args...) {
    return ::eventJump(args...);
}

mixed direct_enter_obj(){
    return 1;
}

mixed direct_enter_into_obj(){
    return direct_enter_obj();
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
}

void init(){
    if(base_name(environment()) == "/domains/town/room/shore"){
        foreach(mixed ident in GetId()){
            if(!ident) continue;
            AddJump(ident,"/domains/town/virtual/surface/5,0",JUMP_INTO);
        }
    }
}

mixed CanGet(object ob) { return "#The sea stays in place.";}

mixed CanEnter(object who, string what) {
    if(base_name(environment()) == "/domains/town/room/shore") return 1;
    return 0;
}

int eventEnter(object who) {
    if( !who ) return 0;
    who->eventPrint("You enter the sea.");
    who->eventMoveLiving("/domains/town/virtual/surface/5,0",
            "$N enters the sea.",
            "$N arrives.");
    return 1;
}

string GetEnter(){
    if(base_name(environment()) == "/domains/town/room/shore"){
        return("/domains/town/virtual/surface/5,0");
    }
    return 0;
}
