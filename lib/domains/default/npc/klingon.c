#include <lib.h>

inherit LIB_SENTIENT;

static void create() {
    sentient::create();
    SetKeyName("klingon");
    SetAdjectives( ({"klingon", "adolescent", "unpredictable"}) );
    SetId( ({"warrior", "fighter", "adolescent"}) );
    SetShort("an adolescent klingon");
    SetLong("Sometimes more dangerous than the adult klingon, adolescents have something to prove and can be unpredictable in their aggression. This one has the distinctive ridges of the house of D'Ghor, making him all the more eager to prove his worthiness as a fighter.");
    SetClass("fighter");
    SetLevel(5);
    SetMelee(1);
    SetRace("klingon");
    SetGender("male");
}
void init(){
    ::init();
}
