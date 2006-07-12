#include <lib.h>

inherit LIB_BARKEEP;
inherit "/domains/fort/etc/call_cops";

static void create() {
    barkeep::create();
    SetKeyName("henrik");
    SetId( ({"fishmonger"}) );
    SetShort("Henrik the fishmonger");
    SetLevel(5);
    SetLong("Henrik is a stocky man with peculiar features, like the people who live across the eastern sea. He's well known for his delicious fresh fish and his fastidiousness in keeping his shop neat and tidy. You can ask him what he has for sale today this way: ask Henrik to list");
    SetInventory(([
	"/domains/fort/armor/smock" : "wear smock",
	"/domains/fort/armor/sandal_l" : "wear left sandal on left foot",
	"/domains/fort/armor/sandal_r" : "wear right sandal on right foot",
	"/domains/fort/weap/filet_knife" : "wield knife",
      ]));
    SetMenuItems(([
	({ "herring", "tinned herring", "herring tin", "tin of herring" }) : "/domains/fort/meals/herring",
	({ "tuna", "tuna fish" }) : "/domains/fort/meals/tuna",
	({ "salmon" }) : "/domains/fort/meals/salmon",
      ]));
    SetProperties(([
	"no bump" : 1,
      ]));
    SetGender("male");
    SetRace("human");
    SetSkill("bargaining", 1);
    SetLocalCurrency("silver");
}
void init(){
    ::init();
}
