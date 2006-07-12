#include <lib.h>

inherit LIB_SENTIENT;

static void create() {
    sentient::create();
    SetKeyName("wulkan");
    SetId( ({"dwarf", "smith"}) );
    SetAdjectives(({"non-player", "non player"}));
    SetShort("Wulkan the smith");
    SetLong("This thick-bearded, stout, swarthy dwarf is the local smith, and has a reputation for toughness, dedication, and craftsmanship. It's said that his rough manners were too much even for his fellow dwarves, but whatever the reason for leaving his mountain home, the people of the Fortress are happy to have him work here.");
    SetInventory(([
	"/domains/fort/armor/canvas_shorts" : "wear shorts",
	"/domains/fort/armor/apron" : "wear apron",
	"/domains/fort/armor/workboot_r" : "wear right boot on right foot",
	"/domains/fort/armor/workboot_l" : "wear left boot on left foot",
	"/domains/fort/weap/poker" : "wield poker in left hand",
	"/domains/fort/weap/wulkan_hammer" : "wield hammer in right hand",
      ]));
    SetClass("fighter");
    SetLevel(5);
    SetMelee(1);
    SetRace("dwarf");
    SetGender("male");
    SetAction(15, ({
	"Wulkan loudly hammers on an anvil.",
	"The forge roars as Wulkan pumps the bellows.", 
	"Wulkan pokes at the forge's furnace and the fire roars.",
	"!say If only I could get my hands on some raw mithril!",
	"Wulkan roars with gusto as he pounds on red-hot steel!",
      }));
     SetCommandResponses( ([
	({ "list", "show", "browse" }) : "I'm no stinking vendor!",
	({ "sell", "serve" }) : "Go flark yourself, wretch!",
      ]) );

}
void init(){
    ::init();
}
