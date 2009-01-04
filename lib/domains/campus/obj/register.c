#include <lib.h>

inherit LIB_STORAGE;

void create() {
    ::create();
    SetKeyName("register");
    SetId(({"register","machine"}));
    SetAdjectives(({"cash","metal"}));
    SetShort("a cash register");
    SetLong("This is a metal machine used for cash transactions.");
    SetMass(200);
    SetBaseCost("silver",50);
    SetMaxCarry(10);
    SetPreventPut("You cannot put this in there!");
    SetMoney( ([
                "dollars" : random(200),
                ]) );
    SetCanClose(1);
    SetClosed(1);
}

void init(){
    ::init();
}
