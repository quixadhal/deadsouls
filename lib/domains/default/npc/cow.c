#include <lib.h>

inherit LIB_SENTIENT;

static void create() {
    sentient::create();
    SetKeyName("cow");
    SetId(({"npc","mob","character","mobile"}));
    SetAdjectives(({"non-player", "non player"}));
    SetShort("a shorthorn waygu cow");
    SetLong("Known far and wide for its exquisite marbling, the meat from the Waygu cow is prized by connoisseurs as a delicacy. This docile animal is used to being pampered, well-fed, and massaged thrice daily.");
    SetWimpy(80);
    SetWanderSpeed(7);
    SetLevel(1);
    SetMelee(1);
    SetRace("cow");
    SetGender("female");
    SetAction(1, ({ "!emote rit." }) );
}
void init(){
    ::init();
}
