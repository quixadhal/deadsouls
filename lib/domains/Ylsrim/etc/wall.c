/*    /domains/Ylsrim/etc/wall.c
 *    From the Dead Souls V Object Library
 *    An example of climbing up
 *    Created by Descartes of Borg 961222
 */

#include <lib.h>
#include <climb.h> // defines CLIMB_UP

inherit LIB_DUMMY; // These do not show up in desc, but you can look at them
inherit LIB_CLIMB; // This makes it climbable

static void create() {
    dummy::create();
    SetKeyName("wall");
    SetId("wall");
    SetAdjectives("adobe", "bank");
    SetShort("an adobe wall");
    SetLong("It has enough holes that you can get a good foot hold and "
	    "climb up it!");
    SetClimb("/domains/Ylsrim/room/bank_roof", CLIMB_UP);
}
