#include <lib.h>

inherit LIB_STORAGE;

void create() {
    ::create();
    SetKeyName("bag");
    SetAdjectives( ({"cloth", "coin"}) );
    SetId( ({ "bag" }) );
    SetShort("a cloth bag");
    SetLong("It is a simple cloth bag used to hold things. ");
    SetMass(50);
    SetBaseCost("silver",1);
    SetMaxCarry(500);
    SetMoney( ([
                "gold" : 5,
                "silver" : 15,
                "copper" : 10,
                "electrum" : 100,
                "platinum" : 1,
                "silver" : 20,
                ]) );
}

void init(){
    ::init();
}
