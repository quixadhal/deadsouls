#include <lib.h>

inherit LIB_STORAGE;

void create() {
    ::create();
    SetKeyName("bag");
    SetAdjectives( ({"small", "cloth", "coin"}) );
    SetId( ({ "bag" }) );
    SetShort("a small cloth bag");
    SetLong("It is a simple cloth bag used to hold things. ");
    SetMass(274);
    SetBaseCost("silver",1);
    SetMaxCarry(50);
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
