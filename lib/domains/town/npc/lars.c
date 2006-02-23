#include <lib.h>

inherit LIB_BARKEEP;

static void create() {
    barkeep::create();
    SetKeyName("lars");
    SetId( ({ "lars", "owner", "barkeep", "bartender", "keeper" }) );
    SetShort("Lars, the keeper of the pub");
    SetLevel(1);
    SetLong("Lars is the owner of the local pub, an old and famous "
      "drinking establishment in the heart of town.  He is an "
      "unassuming fellow, and quite jovial.");
    SetMenuItems(([
	({ "espresso", "imported espresso" }) : "/domains/town/meals/espresso",
	({ "sandwich", "ham sandwich" }) : "/domains/town/meals/ham_sand",
	({ "spring water", "water" }) : "/domains/town/meals/water",
	({ "first class ale", "ale" }) : "/domains/town/meals/ale",
	({ "regular coffee", "coffee" }) : "/domains/town/meals/coffee",
	({ "firebreather" }) : "/domains/town/meals/fire",
	({ "special of the house", "special" }) : "/domains/town/meals/special",
      ]));
    SetGender("male");
    SetMorality(40);
    SetRace("human");
    AddCurrency("electrum", random(200));
    SetSkill("bargaining", 1);
    SetProperty("no bump", 1);
    SetLocalCurrency("silver");
}
void init(){
    ::init();
}
