#include <lib.h>

inherit LIB_BARKEEP;

static void create() {
    barkeep::create();
    SetKeyName("barkeep");
    SetId( ({ "barman" }) );
    SetShort("a generic barkeep");
    SetLevel(1);
    SetLong("A nondescript being whose job is to sell food and drink.");
    SetMenuItems(([
                ]));
    SetGender("male");
    SetRace("human");
    SetSkill("bargaining", 1);
    SetProperty("no bump", 1);
    SetLocalCurrency("silver");
}
void init(){
    ::init();
}
