#include <position.h>
#include <lib.h>

inherit LIB_SENTIENT;

static void create() {
    sentient::create();
    SetKeyName("spectre");
    SetAdjectives( ({"shadowy", "undead", "unholy", "malevolent", "spiteful"}) );
    SetId( ({"wraith", "specter", "ghost", "apparition", "manifestation"}) );
    SetShort("a sinister spectre");
    SetLong("This shadowy manifestation is an undead, unholy apparition, oozing malevolence and spite.");
    SetPosition(POSITION_FLYING);
    SetLevel(15);
    SetMelee(1);
    SetRace("wraith");
    SetGender("neuter");
    SetUndead(1);
}
void init(){
    ::init();
}
