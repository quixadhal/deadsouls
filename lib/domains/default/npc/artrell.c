#include <lib.h>

inherit LIB_SENTIENT;

static void create() {
    sentient::create();
    SetKeyName("artrell");
    SetAdjectives( ({"insectoid"}) );
    SetId(({"npc","mob","character","mobile"}));
    SetShort("an artrell");
    SetLong("This creature would be roughly humanoid if it weren't for its insectoid appearance and four arms. It appears slightly thinner and smaller than an adult human, but you still don't want to touch it. It's the biggest damned bug you've ever seen.");
    SetLevel(1);
    SetRace("artrell");
    SetGender("male");
}
void init(){
    ::init();
}
