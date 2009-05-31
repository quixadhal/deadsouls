/*    /domains/Ylsrim/npc/balrog.c
 *    From the Dead Souls Mud Library
 *    An example simple NPC
 *    Created by Descartes of Borg 960302
 */

#include <lib.h>

inherit LIB_NPC;

static void create() {
    npc::create();
    SetKeyName("balrog");
    SetId("balrog");
    SetAdjectives("wicked", "ugly");
    SetShort("a wicked balrog");
    SetLong("Balrogs are an ugly sort that hide out among the many "
            "crevices and canyons in the mountains.  This one is "
            "extremely tall, and evidently not all too pleased to see you.");
    SetLevel(8);         // level, race, and class determine NPC's power
    SetRace("balrog");   // must be some valid race from mraces command
    SetClass("fighter"); // needs a class!
    SetGender("male");
    SetEncounter(80);
    SetMorality(-800);   // -2000 absolute evil, 2000 absolute good
    SetAction(5, ({ "!growl" })); // Make him growl every now and then
}
