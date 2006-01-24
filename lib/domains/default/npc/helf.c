#include <lib.h>

inherit LIB_SENTIENT;

static void create() {
    sentient::create();
    SetKeyName("optis");
    SetId( ({"half elf", "half-elf", "half-human", "half human"}) );
    SetAdjectives(({"non-player", "non player"}));
    SetShort("Optis the half-elf");
    SetLong("Like most of his kind, Optis is very attractive and athletic. His ears aren't entirely pointy, but he's exotic enough to be very popular with the ladies.");
    SetLevel(1);
    SetMelee(1);
    SetRace("half-elf");
    SetGender("male");
}
void init(){
    ::init();
}
