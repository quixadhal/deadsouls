#include <lib.h>

inherit LIB_SENTIENT;


static void create() {
    sentient::create();
    SetKeyName("Miles");
    SetId(({"miles"}));
    SetShort("Miles");
    SetLong("Miles is a foreign exchange student from the United Kingdom.");
    SetLevel(5);
    SetRace("human");
    SetGender("male");
    SetNoClean(1);
}
void init(){
    ::init();
}
