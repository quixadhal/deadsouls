/*    /domains/Ylsrim/etc/shovel.c
 *    From the Dead Souls V Object Library
 *    An example of how to code a digging tool
 *    Created by Descartes of Borg 961231
 */

#include <lib.h>

inherit LIB_ITEM;
inherit LIB_DIG_WITH;

static void create() {
    item::create();
    SetKeyName("shovel");
    SetId("shovel", "spade");
    SetAdjectives("wooden");
    SetShort("a shovel");
    SetLong("A plain, wooden shovel.  You probably can dig things with "
	    "it.");
    SetMass(87);
    SetValue(50);
}
