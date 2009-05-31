#include <lib.h>

inherit LIB_SENTIENT;


static void create() {
    sentient::create();
    SetKeyName("Ted");
    SetId(({"ted"}));
    SetShort("Ted");
    SetLong("Ted is a student in the Virtual Campus.");
    SetLevel(5);
    SetRace("human");
    SetGender("male");
    SetNoClean(1);
}
void init(){
    ::init();
}
