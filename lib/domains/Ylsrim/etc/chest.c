/*    /domains/Ylsrim/etc/chest.c
 *    from the Dead Souls V Object Library
 *    created by Descartes of Borg 951028
 */

#include <lib.h>

inherit LIB_STORAGE;

static void create() {
    storage::create();
    SetKeyName("chest");
    SetId( ({ "chest" }) );
    SetAdjectives( ({ "wooden" }) );
    SetShort("a wooden chest");
    SetLong("This chest is made from an ancient forest far from "
	    "Ylsrim.  You can feel the age in touching it.");
    SetMass(1200);
    SetValue(300);
    SetInventory( ([ DIR_STANDARD_DOMAIN "/etc/shovel" : -10,
		    DIR_STANDARD_DOMAIN "/etc/heal_scroll" : 1 ]) );
    SetCanClose(1);
    SetClosed(1);
    SetMaxCarry(2000);
}
