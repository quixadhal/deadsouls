#include <position.h>
#include <lib.h>

inherit LIB_SENTIENT;

static void create() {
    sentient::create();
    SetKeyName("slug");
    SetId( ({"gastropod"}) );
    SetAdjectives( ({"big", "thick", "slimy", "garden", "gross"}) );
    SetShort("a slimy slug");
    SetLong("A big, thick, slimy garden slug. Gross!");
    SetAutoStand(0);
    SetPosition(POSITION_LYING);
    SetLevel(1);
    SetMelee(1);
    SetRace("slug");
    SetMaxHealthPoints(5);
    SetGender("male");
}
void init(){
    ::init();
}
