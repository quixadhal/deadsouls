#include <lib.h>

inherit LIB_SENTIENT;

static void create() {
    sentient::create();
    SetKeyName("elephant");
    SetAdjectives( ({"docile", "pink", "friendly", "large", "very large"}) );
    SetId(({"npc","mob","character","mobile"}));
    SetShort("a pink elephant");
    SetLong("This is a very large creature, yet it seems quite docile and even friendly. It's unclear whether the pink color is natural or painted on, but it's quite becoming.");
    SetLevel(1);
    SetMelee(1);
    SetRace("elephant");
    SetGender("female");
}
void init(){
    ::init();
}
