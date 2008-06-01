#include <lib.h>
#include <position.h>

inherit LIB_SENTIENT;

static void create() {
    sentient::create();
    SetKeyName("fish");
    SetAdjectives( ({"cod", "medium", "unremarkable"}) );
    SetId(({"cod","codfish"}));
    SetShort("a fish");
    SetLong("A codfish of medium size.");
    SetWanderSpeed(1);
    //Letting npc's load virtual rooms can be dangerous.
    //You were warned.
    //SetPermitLoad(1);
    SetPosition(POSITION_SWIMMING);
    SetPacifist(1);
    SetLevel(1);
    SetRace("fish");
    SetHealthPoints(10);
    SetMaxHealthPoints(10);
    SetGender("male");
}
void init(){
    ::init();
}
