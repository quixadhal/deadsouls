/*    /domains/Praxis/etc/basement_door.c
 *    from Dead Souls LPMud
 *    created by Descartes of Borg 951027
 */

#include <lib.h>

inherit LIB_DOOR;

static void create() {
    door::create();
    SetId("north", "door");
    SetShort("north", "a door made from fine oak");
    SetLong("north", "It is a magnificent door marking the entrance to "
	    "the fighter class hall.");
    SetLockable("north", 1);
    SetKeys("north", "special_key_id");
    SetId("south", "door");
    SetShort("south", "a door made from fine oak");
    SetLong("south", "It is a magnificent door leading out to the recruitment "
	    "area.");
    SetLockable("south", 1);
    SetKeys("south", "special_key_id");
    SetClosed(1);
    SetLocked(1);
}
