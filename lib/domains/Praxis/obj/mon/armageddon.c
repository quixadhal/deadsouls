#include <lib.h>
inherit "/lib/npc";

#include <daemons.h>

void create() {
    ::create();
    SetKeyName("armageddon");
    SetId( ({ "armageddon", "crasher", "asshole", "game crasher", "armageddon the game crasher" }) );
    SetShort("Game crasher Armageddon of Borg");
    SetLong("He appears once and briefly only to cause doom to the mud.\n");
    SetLevel(33);
    SetHealthPoints(10000);
    SetRace("human");
}

