#include <lib.h>
#include <position.h>

inherit LIB_SENTIENT;

static void create() {
    sentient::create();
    SetKeyName("fish");
    SetAdjectives( ({"grouper", "large", "tall","flat"}) );
    SetId(({"grouper"}));
    SetShort("a grouper fish");
    SetLong("A very large fish with a huge mouth and bizarrely tall, flat body.");
    SetWanderSpeed(3);
    //Letting npc's load virtual rooms can be dangerous.
    //You were warned.
    //SetPermitLoad(1);
    SetPosition(POSITION_SWIMMING);
    SetPacifist(1);
    SetLevel(10);
    SetRace("fish");
    SetGender("male");
}
void init(){
    ::init();
}
