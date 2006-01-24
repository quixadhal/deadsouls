#include <lib.h>

inherit LIB_SENTIENT;

static void create() {
    sentient::create();
    SetKeyName("boar");
    SetAdjectives( ({"huge", "tusked", "hairy", "deadly", "snorting", "sweaty", "ugly", "piggish"}) );
    SetId( ({"pig", "beast"}) );
    SetShort("a wild boar");
    SetLong("This huge, tusked, hairy beast is at least one thousand pounds of snorting, sweaty aggression. Though it's ugly and piggish, it's every inch as deadly to a person as any lion, tiger, or bear.");
    SetLevel(1);
    SetMelee(1);
    SetRace("pig");
    SetGender("male");
}
void init(){
    ::init();
}
