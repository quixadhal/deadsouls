#include <lib.h>

inherit LIB_DUMMY; // These do not show up in desc, but you can look at them

static void create() {
    dummy::create();
    SetKeyName("wall");
    SetId(({"elevator","door","wall" }));
    SetAdjectives("call", "elevator","west");
    SetShort("a wall");
    SetLong("Set into the west wall is a thoroughly modern-looking "
            "elevator, which seems out of place in this rustic, colonial "
            "church. There is a button next to it, presumably to call "
            "the elevator car.");
    AddItem(new("/domains/town/obj/church_button"));
}
