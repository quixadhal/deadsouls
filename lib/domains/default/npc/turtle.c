#include <lib.h>

inherit LIB_SENTIENT;

static void create() {
    sentient::create();
    SetKeyName("mock turtle");
    SetAdjectives( ({"peculiar", "mock"}) );
    SetId( ({"turtle", "tortoise"}) );
    SetShort("a mock turtle");
    SetLong("This peculiar creature has the head of a calf but the body of a turtle.");
    SetLevel(1);
    SetMelee(1);
    SetRace("tortoise");
    SetGender("male");
}
void init(){
    ::init();
}
