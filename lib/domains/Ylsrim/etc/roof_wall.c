/*    /domains/Ylsrim/etc/roof_wall.c
 *    From the Dead Souls Mud Library
 *    An example of climbing down
 *    Created by Descartes of Borg 961222
 */

#include <lib.h>
#include <climb.h> // defines CLIMB_DOWN

inherit LIB_DUMMY; // These do not show up in desc, but you can look at them
inherit LIB_CLIMB; // This makes it climbable

static void create() {
    dummy::create();
    SetKeyName("wall");
    SetId("wall");
    SetAdjectives("adobe", "bank");
    SetShort("an adobe wall");
    SetLong("It has enough holes that you can get a good foot hold and "
            "climb down it!");
    SetClimb("/domains/Ylsrim/room/kaliid6", CLIMB_DOWN);
}
