/*    /domains/Ylsrim/npc/priest.c
 *    From the Dead Souls V Object Library
 *    An example of a religious class leader
 *    Created by Descartes of Borg 961222
 */

#include <lib.h>

inherit LIB_LEADER;

static void create() {
    leader::create();
    SetKeyName("ixtala");
    SetId("ixtala", "priest");
    SetShort("Ix'Tala, high priestess of the Priests");
    SetLong("Ix'Tala initiates priests into the Priests class as well as "
	    "converts the wayward to the truth of the Dalin religion.  "
	    "To learn more about becoming a priest "
	    "ask her to \"describe priests\".  If you choose to become "
	    "a priest, ask her to \"join priests\".  Of course, you can "
	    "convert by asking her to \"convert me\".");
    SetLevel(45);
    SetRace("klingon");
    SetClass("priest");
    SetGender("female");
    SetMorality(400);
    SetReligion("Dalin", "Dalite");
    SetProperty("no bump", 1);
    if( clonep() ) {
	AddChannel("priest");
    }
}
