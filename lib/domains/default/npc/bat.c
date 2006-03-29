#include <position.h>
#include <lib.h>

inherit LIB_SENTIENT;

static void create() {
    sentient::create();
    SetKeyName("bat");
    SetId(({"npc","mob","character","mobile"}));
    SetAdjectives(({"non-player", "non player"}));
    SetShort("a little bat");
    SetLong("An ordinary bat. It's no bigger than a mouse.");
    SetWimpy(80);
    SetPermitLoad(1);
    SetPosition(POSITION_FLYING);
    SetWanderSpeed(1);
    SetLevel(1);
    SetMelee(1);
    SetRace("bat");
    SetMaxHealthPoints(20);
    SetGender("male");
    SetMessage("leave","$N scurries $D.");
}
void init(){
    ::init();
}
