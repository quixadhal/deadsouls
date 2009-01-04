#include <lib.h>

inherit LIB_SENTIENT;


static void create() {
    sentient::create();
    SetKeyName("Graham");
    SetId(({"graham"}));
    SetShort("Graham");
    SetLong("Graham is a foreign exchange student from the United Kingdom.");
    SetLevel(5);
    SetRace("human");
    SetGender("male");
    SetNoClean(1);
}
void init(){
    ::init();
}
