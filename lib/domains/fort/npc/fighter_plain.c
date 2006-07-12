#include <lib.h>
#include "../customdefs.h"

inherit LIB_SENTIENT;

static void create(){
    sentient::create();
    SetKeyName("fighter");
    SetId( ({"human","npc","warrior","fighter"}) );
    SetShort("a fighter");
    SetLong("This is a sample warrior SENTIENT.");
    SetLevel(10);
    SetRace("human");
    SetClass("fighter");
    SetGender("male");
}
void init(){
    ::init();
}
