#include <lib.h>

inherit LIB_SENTIENT;

static void create() {
    sentient::create();
    SetKeyName("gargoyle");
    SetAdjectives( ({"stone", "grotesque", "ugly", "repellent"}) );
    SetId(({"npc","mob","character","mobile"}));
    SetShort("a grotesque gargoyle");
    SetLong("A stone spirit come to life, this monstrosity was designed to be as ugly and repellent as anything could be and still hold its shape. It is wildly successful. Its rolling eyes and lolling tongue make it a horrid sight indeed.");
    SetLevel(1);
    SetMelee(1);
    SetRace("gargoyle");
    SetGender("male");
}
void init(){
    ::init();
}
