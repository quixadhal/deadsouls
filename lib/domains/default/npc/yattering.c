#include <lib.h>

inherit LIB_SENTIENT;

static void create() {
    sentient::create();
    SetKeyName("yattering");
    SetId(({"npc","mob","character","mobile"}));
    SetAdjectives(({"non-player", "non player"}));
    SetShort("a foul little yattering");
    SetLong("This is a wretched, fleshy pink creature about two feet tall. It has long, droopy ears, a beaklike mouth, and a pair of useless wing stubs on its back. Its general demeanor is listless and somehow resentful. As pathetic as it looks, its possesses long black claws, and its beak looks plenty sharp.");
    SetClass("fighter");
    SetLevel(5);
    SetMelee(1);
    SetRace("demon");
    SetGender("male");
}
void init(){
    ::init();
}
