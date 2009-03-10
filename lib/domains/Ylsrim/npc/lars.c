/*    /domains/Ylsrim/npc/lars.c
 *    From the Dead Souls Mud Library
 *    An example barkeeper
 *    Created by Descartes of Borg 960302
 */

#include <lib.h>
#include <dirs.h>

inherit LIB_BARKEEP;

static void create() {
    barkeep::create();
    SetKeyName("lars");
    SetId("lars", "owner", "barkeep", "bartender", "keeper");
    SetShort("Lars, the keeper of Lars' Pub");
    SetLevel(12);
    SetLong("Lars is the owner of Lars' Pub, and old, famous "
            "drinking establishment in the heart of the Ylsrim Bazaar.  "
            "He is an unassuming elf, but quite jovial.  You may ask him to "
            "serve whatever drinks are on his menu.");
    SetGender("male");
    SetMorality(40);
    SetRace("elf");
    SetClass("fighter");
    AddCurrency("electrum", random(200));
    // How good a barkeep is he?
    SetSkill("bargaining", 150);
    // Keep him in the pub
    SetProperty("no bump", 1);
    // He takes electrum
    SetLocalCurrency("electrum");
    // What he sells
    SetMenuItems(([
                ({ "ale", "beer" }) : "/domains/Ylsrim/meal/ale",
                ]));
}
void init(){
    ::init();
}
