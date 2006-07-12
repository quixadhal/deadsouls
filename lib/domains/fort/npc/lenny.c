#include <lib.h>

inherit LIB_BARKEEP;


static void create() {
    barkeep::create();
    SetKeyName("lenny");
    SetId( ({ "lenny", "owner", "barkeep", "bartender", "keeper" }) );
    SetShort("Lenny, the keeper of the pub");
    SetLevel(1);
    SetLong("Lenny is the owner of the local pub, an old and famous "
      "drinking establishment in the heart of town.  He is an "
      "unassuming fellow, and quite jovial.");
    SetGender("male");
    SetMorality(40);
    SetRace("human");
    AddCurrency("electrum", random(200));
    SetSkill("bargaining", 1);
    SetProperty("no bump", 1);
    SetLocalCurrency("silver");
    SetMenuItems(([
	({"first class ale","ale"}) : "/domains/town/meals/ale",
	"water" : "/domains/town/meals/water",
	"spring water" : "/domains/town/meals/water",
	"ham sandwich" : "/domains/town/meals/ham_sand",
	"sandwich" : "/domains/town/meals/ham_sand",
	"special" : "/domains/town/meals/special",
	"special of the house" : "/domains/town/meals/special",
	"firebreather" : "/domains/town/meals/fire",
	"coffee" : "/domains/town/meals/coffee",
	"regular coffee" : "/domains/town/meals/coffee",
	"imported espresso" : "/domains/town/meals/espresso",
	"espresso" : "/domains/town/meals/espresso"
      ]) );
}
void init(){
    ::init();
}