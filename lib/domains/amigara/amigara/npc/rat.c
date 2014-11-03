#include <lib.h>

inherit LIB_SENTIENT;

static void create() {
    sentient::create();
    SetKeyName("rat");
    SetAdjectives( ({"ratlike","unusual","large","very","dangerous",
                "dangerous looking","rat like"}) );
    SetId( ({"rat","rous","beast"}) );
    SetShort("a rodent of unusual size");
    SetLong("A very large, very dangerous-looking ratlike beast.");
    SetRace("rodent");
    SetGender("male");
    SetClass("fighter");
    SetEncounter(100);
    SetLevel(1);
    SetMelee(1);
    SetMessage("come","$N scurries in.");
    SetMessage("leave","$N scurries $D.");
    SetGuard("/domains/town/obj/ladder","The rat snarls and blocks your attempt.");
}

void init(){
    ::init();
}

void heart_beat(){
    object env = environment();
    ::heart_beat();
}

