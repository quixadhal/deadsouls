/*    /domains/Ylsrim/etc/dug_hole.c
 *    From the Dead Souls Mud Library
 *    A door that is really a hole instead of a door
 *    Created by Descartes of Borg 961231
 */

#include <lib.h>

inherit LIB_DOOR;

string checkOpenDown(object who) {
    if( GetClosed() ) {
        return "You would need to dig a hole.";
    }
    else {
        return "It is a deep, dark hole in the desert sand.";
    }
}

string checkOpenUp(object who) {
    if( GetClosed() ) {
        return "There is no hole here.";
    }
    else {
        return "It leads into the open air.";
    }
}

mixed CanClose() {
    return 0; // people should not be closing this manually
}

mixed CanOpen() {
    return 0; // people should not be opening this manually
}

static void create() {
    door::create();
    SetId("down", "hole");
    SetShort("down", "sand"); // this is what is seen when bumped into
    SetLong("down", (: checkOpenDown :));
    SetId("up", "hole");
    SetShort("up", "sand"); // this is what is seen when bumped into
    SetLong("up", (: checkOpenUp :));
    SetClosed(1);
}
