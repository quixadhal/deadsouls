#include <lib.h>

inherit LIB_DUMMY; // These do not show up in desc, but you can look at them

static void create() {
    dummy::create();
    SetKeyName("lamp");
    SetId("lamp on a post","post","lamp post","lamppost");
    SetShort("a lamp on a post");
    SetLong("This is a lamp on a post which is lit at night so townsfolk "
            "can find their way around.");
}
