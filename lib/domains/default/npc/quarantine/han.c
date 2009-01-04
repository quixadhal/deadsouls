#include <lib.h>

inherit LIB_SENTIENT;

static void create() {
    sentient::create();
    SetKeyName("Han");
    SetId(({"han"}));
    SetShort("Han");
    SetLong("Han is a foreign exchange student from the Netherlands.");
    SetMelee(1);
    SetLevel(5);
    SetRace("human");
    SetGender("male");
    SetNoClean(1);
}
void init(){
    ::init();
}
