#include <lib.h>

inherit LIB_SENTIENT;

static void create() {
    sentient::create();
    SetKeyName("rat");
    SetAdjectives( ({"mangy", "little", "dirty"}) );
    SetId( ({"rat"}) );
    SetShort("a rat");
    SetLong("A scruffy little dirty rat.");
    SetRace("rodent");
    SetHealthPoints(3);
    SetMaxHealthPoints(3);
    SetGender("male");
    SetEncounter(100);
    SetMessage("come","$N scurries in.");
    SetMessage("leave","$N scurries $D.");
    SetAction(5, ({ 
        "The rat squeaks.", "You hear a rat scuttling about.", 
        "A scruffy little rat brushes against your leg.",
        "You hear tiny munching sounds."}));
}
void init(){
    ::init();
}
