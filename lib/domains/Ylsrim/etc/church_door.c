/*    /domains/Ylsrim/etc/church_door.c
 *    From the Dead Souls V Object Library
 *    An example of a simple door
 *    Created by Descartes of Borg 951027
 */

#include <lib.h>

inherit LIB_DOOR;

static void create() {
    door::create();
    SetId("church", "door");
    SetShort("church", "a door made from old wood");
    SetLong("church", "It is a large door that looks like it could fall "
	    "apart any minute.");
    SetLockable("church", 1);
    SetKeys("church", "#nothing_can_unlock_me#");
    SetId("south", "door");
    SetShort("south", "a door made from old wood");
    SetLong("south", "It is a large door that looks like it could fall "
	    "apart any minute.");
    SetLockable("south", 1);
    SetKeys("south", "#nothing_can_unlock_me#");
    SetClosed(1);
    SetLocked(1);
}
