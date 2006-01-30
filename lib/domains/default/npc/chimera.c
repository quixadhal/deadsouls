#include <lib.h>

inherit LIB_SENTIENT;

static void create() {
    sentient::create();
    SetKeyName("chimera");
    SetId(({"npc","mob","character","mobile"}));
    SetAdjectives(({"non-player", "non player"}));
    SetShort("a terrifying chimera");
    SetLong("This terrible creature is an unholy mix consisting of the head and body of a lion, a head of a dragon, another head of a goat, and a serpent for a tail. Its heads bob ceaselessly, eyes rolling madly in their sockets, mouths opening and snapping shut.");
    SetClass("fighter");
    SetLevel(15);
    SetMelee(1);
    SetRace("chimera");
    SetGender("male");
}
void init(){
    ::init();
}
