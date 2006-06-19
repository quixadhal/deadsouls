#include <lib.h>

inherit LIB_SENTIENT;

static void create() {
    sentient::create();
    SetKeyName("Wim");
    SetId(({"wim"}));
    SetShort("Wim");
    SetLong("Wim is a foreign exchange student from the Netherlands.");
    SetLevel(5);
    SetRace("human");
    SetGender("male");
}
void init(){
    ::init();
}
