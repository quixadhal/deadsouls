#include <lib.h>

inherit LIB_SENTIENT;

static void create() {
    sentient::create();
    SetKeyName("sheep");
    SetId(({"npc","mob","character","mobile"}));
    SetAdjectives(({"non-player", "non player"}));
    SetShort("a sheep");
    SetLong("This is a typically placid mammal ungulate, given to wandering fields munching on grass. Its fur is called wool and is unusually thick and warm. Wool is prized as a component in clothing.");
    SetLevel(1);
    SetMelee(1);
    SetRace("sheep");
    SetGender("male");
}
void init(){
    ::init();
}
