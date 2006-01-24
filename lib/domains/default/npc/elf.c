#include <lib.h>

inherit LIB_SENTIENT;

static void create() {
    sentient::create();
    SetKeyName("elf");
    SetAdjectives( ({"arrogant", "smug", "malicious"}) );
    SetId(({"npc","mob","character","mobile"}));
    SetShort("an arrogant elf");
    SetLong("Oh don't be fooled. Elves just love to pretend they believe in equality and so on, but they all secretly harbor a malicious joy in being oh so much smarter and long-lived and wiser than humans. Don't you just want to punch this elf's smug little smirk?");
    SetLevel(1);
    SetMelee(1);
    SetRace("elf");
    SetGender("male");
}
void init(){
    ::init();
}
