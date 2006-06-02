#include <lib.h>

inherit LIB_SENTIENT;

static void create() {
    sentient::create();
    SetKeyName("rat");
    SetAdjectives( ({"mangy", "little", "dirty"}) );
    SetId( ({"rat"}) );
    SetShort("a rat");
    SetLong("A scruffy little dirty rat.");
    SetLevel(3);
    SetRace("rodent");
    SetGender("male");
    SetClass("fighter");
    SetMaxHealthPoints(5);
    SetWanderSpeed(1);
    SetEncounter(100);
    SetMessage("come","$N scurries in.");
    SetMessage("leave","$N scurries $D.");
    SetAction(5, ({ 
	"The rat squeaks.", "You hear a rat scuttling about.", 
	"A scruffy little rat brushes against your leg.",
	"You hear tiny munching sounds."}));
}
void init(){
    SetMaxHealthPoints(3);
    ::init();
}
