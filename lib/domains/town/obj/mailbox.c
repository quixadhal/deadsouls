#include <lib.h>

inherit LIB_STORAGE;

void create() {
    ::create();
    SetKeyName("mailbox");
    SetId( ({"mailbox", "box"}) );
    SetAdjectives( ({ "generic","nondescript" }) );
    SetShort("a small mailbox");
    SetLong("This is a small mailbox standing in front of the white house.");
    SetCanClose(1);
    SetMass(274);
    SetBaseCost("silver",1);
    SetMaxCarry(100);
    SetPreventGet("You cannot get that!");
}

void init(){
    ::init();
}
