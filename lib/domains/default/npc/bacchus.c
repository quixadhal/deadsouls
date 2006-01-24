#include <lib.h>

inherit LIB_SENTIENT;

static void create() {
    sentient::create();
    SetKeyName("bacchus");
    SetAdjectives( ({"roman", "drunk"}) );
    SetId( ({"god", "god of wine", "dionysos", "dionysus"}) );
    SetShort("Bacchus, Roman god of wine");
    SetLong("How the Roman god of wine found his way here is a mystery, probably most so to himself. At the moment he seems to be in desperate need of sleeping off his latest debauch. He'll have to figure out an explanation of his own for waking up with a hangover in a crowd of animals.");
    SetLevel(200);
    SetMelee(1);
    SetRace("god");
    SetGender("male");
    SetSleeping(500);
}
void init(){
    ::init();
}
