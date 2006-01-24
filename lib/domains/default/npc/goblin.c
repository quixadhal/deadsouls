#include <lib.h>

inherit LIB_SENTIENT;

static void create() {
    sentient::create();
    SetKeyName("goblin");
    SetAdjectives( ({"malicious", "short", "green", "clever"}) );
    SetId(({"npc","mob","character","mobile"}));
    SetShort("a short, green goblin");
    SetLong("This awful creature is confirmation of the worst stories you heard as a child. It seems to ooze malice from its slick, green skin. It looks much like a troll but smaller and more clever, and the eyes almost appear to radiate poison and hatred. Its very presence contaminates the air.");
    SetLevel(1);
    SetMelee(1);
    SetRace("goblin");
    SetGender("male");
}
void init(){
    ::init();
}
