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
    SetMass(274);
    SetBaseCost("silver",1);
    SetMaxCarry(500);
}
