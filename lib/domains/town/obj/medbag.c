#include <lib.h>

inherit LIB_STORAGE;

void create() {
    ::create();
    SetKeyName("bag");
    SetId( ({ "medical bag" }) );
    SetAdjectives( ({ "small", "cloth", "a" }) );
    SetShort("a small cloth bag");
    SetLong("It is a simple cloth bag used to hold things. Printed on it is: "
            "\"Property of A.S. Clepius\"");
    SetMass(30);
    SetBaseCost("silver",5);
    SetMaxCarry(300);
}

void init(){
    ::init();
}
