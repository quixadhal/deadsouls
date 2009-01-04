#include <lib.h>

inherit LIB_SENTIENT;

static void create() {
    sentient::create();
    SetKeyName("Simon");
    SetId(({"simon"}));
    SetShort("Simon");
    SetLong("Simon is a foreign exchange student from the United Kingdom.");
    SetLevel(5);
    SetRace("human");
    SetGender("male");
    SetNoClean(1);
}
void init(){
    ::init();
}
