#include <lib.h>

inherit LIB_SENTIENT;

static void create() {
    sentient::create();
    SetKeyName("halfling");
    SetId(({"npc","mob","character","mobile"}));
    SetAdjectives(({"non-player", "non player"}));
    SetShort("a halfling");
    SetLong("This creature is almost identical in appearance to a hobbit with its short stature, hairy feet, and evidently fun-loving disposition. It typically takes a hobbit or a halfling to tell them apart, but halflings tend to be shunned and disregarded for reasons lost to history, so if it looks a bit poor and underfed, like this one, it's a halfling.");
    SetLevel(1);
    SetMelee(1);
    SetRace("halfling");
    SetGender("male");
}
void init(){
    ::init();
}
