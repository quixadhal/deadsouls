#include <lib.h>

inherit LIB_VEHICLE;
inherit LIB_MOUNT;

static void create() {
    sentient::create();
    SetKeyName("strider");
    SetId(({"landstrider","walker","scout","columbu"}));
    SetShort("a landstrider");
    SetLong("This is a Columbu class scout vehicle consisting of "
      "a human-sized ball turret mounted on two large birdlike legs.");
    SetRace("mech");
    SetMelee(0);
    SetNoClean(1);
}
void init(){
    ::init();
}
