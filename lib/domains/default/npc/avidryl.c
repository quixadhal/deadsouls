#include <position.h>
#include <lib.h>

inherit LIB_SENTIENT;

static void create() {
    sentient::create();
    SetKeyName("avidryl");
    SetId(({"npc","mob","character","mobile"}));
    SetAdjectives(({"non-player", "non player"}));
    SetShort("an avidryl");
    SetLong("This is the fabled avidryl, or 'birdman'. He is very human-like, approximately six feet tall and robust in build. He looks like he might even be a match for a Klingon. The most striking thing about him are the two large birdlike wings on his back, used for flying.");
    SetAutoStand(1);
    SetPosition(POSITION_FLYING);
    SetClass("explorer");
    SetLevel(5);
    SetRace("avidryl");
    SetGender("male");
}
void init(){
    ::init();
}
