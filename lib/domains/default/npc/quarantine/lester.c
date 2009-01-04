#include <lib.h>

inherit LIB_SENTIENT;


static void create() {
    sentient::create();
    SetKeyName("lester");
    SetId(({"lester"}));
    SetShort("Lester");
    SetLong("Lester is a foreign exchange student from the United Kingdom.");
    SetLevel(5);
    SetRace("human");
    SetGender("male");
    SetNoClean(1);
}
void init(){
    ::init();
}
