#include <position.h>
#include <lib.h>

inherit LIB_SENTIENT;

static void create() {
    sentient::create();
    SetKeyName("faerie");
    SetAdjectives( ({"fragile", "beautiful", "incredibly beautiful", "insubstantial", "tiny"}) );
    SetId( ({"fairy", "fairie"}) );
    SetShort("a small, delicate fairy");
    SetLong("This creature looks like a tiny human, perhaps six inches tall, with diaphanous wings. It seems fragile and insubstantial, as well as incredibly beautiful.");
    SetPosition(POSITION_FLYING);
    SetLevel(1);
    SetMelee(1);
    SetRace("faerie");
    SetMaxHealthPoints(50);
    SetGender("female");
}
void init(){
    ::init();
}
