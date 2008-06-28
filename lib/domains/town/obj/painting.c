#include <lib.h>
inherit LIB_ITEM;

static void create(){
    item::create();
    SetKeyName("painting in the room");
    SetId("painting");
    SetAdjectives("beautiful");
    SetLong("Try: look at painting on the wall");
    SetNoCondition(1);
    SetPreventGet("It does not move.");
}

void init(){
    ::init();
}
