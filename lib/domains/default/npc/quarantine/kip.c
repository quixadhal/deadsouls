#include <lib.h>

inherit LIB_NPC;

static void create() {
    npc::create();
    SetKeyName("Kip");
    SetId(({"kip"}));
    SetShort("Kip");
    SetLong("Kip is a foreign exchange student from the Netherlands.");
    SetMelee(1);
    SetLevel(5);
    SetRace("human");
    SetGender("male");
    SetNoClean(1);
}
void init(){
    ::init();
}
