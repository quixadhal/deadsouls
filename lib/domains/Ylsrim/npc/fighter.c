/*    /domains/Ylsrim/npc/fighter.c
 *    From the Dead Souls V Object Library
 *    An example class leader
 *    Created by Lassondra@Dead Souls
 */

#include <lib.h>

inherit LIB_LEADER;

static void create() {
    leader::create();
    SetKeyName("roshd burlyneck");
    SetId("roshd", "roshd burlyneck");
    SetShort("Roshd Burlyneck, Master Warrior");
    SetLong("Roshd Burlyneck is the master warrior of "
	    "the fighters. Immense in size, Roshd stands well "
	    "over seven feet tall. Long brown hair streams "
	    "out behind him, and his biceps are so large "
	    "it would take several hand spans to fit around "
	    "them. Roshd's armour is worn and covered in dirt "
	    "as if he has just returned from combat. "
	    "Roshd initiates new members into the ways of "
	    "combat.  To learn more about becoming a fighter "
	    "ask him to \"describe fighters\".  If you choose to become "
	    "a fighter, ask him to \"join fighters\".");
    SetLevel(45);
    SetRace("klingon");
    SetClass("fighter");
    SetGender("male");
    SetMorality(-400);
    SetProperty("no bump", 1);
    // stick him on the fighter line
    if( clonep() ) {
	AddChannel("fighter");
    }
}
