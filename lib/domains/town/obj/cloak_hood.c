#include <lib.h>

inherit LIB_DUMMY; // These do not show up in desc, but you can look at them
inherit LIB_WEAR; // Makes the item pressable

varargs mixed eventWear(object who) {
    return 1;
}

static void create() {
    dummy::create();
    SetKeyName("hood");
    SetId("cloak hood","cloak's hood");
    SetShort("a hood");
    SetLong("A hood on a cloak.");
    SetInvis(0);
    //SetWear((: eventWear :));
}
