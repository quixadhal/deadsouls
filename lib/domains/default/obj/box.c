#include <lib.h>

inherit LIB_STORAGE;

void create() {
    ::create();
    SetKeyName("box");
    SetId( ({ "box" }) );
    SetAdjectives( ({ "small", "plastic","green", "a" }) );
    SetShort("a small plastic box");
    SetLong("It is a simple plastic box used to hold things.");
    SetMass(274);
    SetMaxCarry(500);
}
