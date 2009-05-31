#include <lib.h>

inherit LIB_BOT;

static void create() {
    ::create();
    SetKeyName("zookeeper bot");
    SetId( ({"bot", "robot", "zookeeper", "keeper"}) );
    SetAdjectives(({"non-player", "non player"}));
    SetShort("a robot zookeeper");
    SetLong("This efficient automaton continually feeds, bathes, grooms, and looks after all the wild things kept in the menagerie.");
    SetLevel(1);
    SetMelee(1);
    SetRace("android");
    SetGender("male");
    SetMaxClones(3);
}
void init(){
    ::init();
}
