#include <lib.h>
inherit LIB_ITEM;
inherit LIB_PRESS;
inherit LIB_MANIPULATE;

static void create() {
    item::create();
    SetKeyName("painting in the room");
    SetId("painting");
    SetAdjectives("beautiful");
    //SetShort("a beautiful painting is on the wall");
    SetLong("Try: look at painting on the wall");
    SetNoCondition(1);
    SetPress( ([ "default" : "Try: push painting on wall" ]) );
    SetManipulate( ([ "default" : "Try: move painting on wall" ]) );
}
mixed CanGet(object ob) { return "Try: get painting on the wall"; }
