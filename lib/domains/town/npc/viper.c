#include <position.h>
#include <lib.h>

inherit LIB_SENTIENT;

static void create() {
    sentient::create();
    SetKeyName("viper");
    SetAdjectives( ({"dangerous", "venomous", "pit"}) );
    SetId( ({"snake", "reptile"}) );
    SetShort("a pit viper");
    SetLong("This is a thickly-built snake, about three feet long, with a brown and black pattern along its muscular body. A member of the family viperidae, it is a class of snake known for the deadliness of its venom.");
    SetAutoStand(0);
    SetPosition(POSITION_LYING);
    SetLevel(3);
    SetMelee(1);
    SetRace("viper");
    SetGender("male");
}
void init(){
    ::init();
}
