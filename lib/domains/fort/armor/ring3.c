#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_ARMOR;

static void create(){
    armor::create();
    SetKeyName("gold_ring");
    SetId(({"ring","gold ring"}));
    SetAdjectives( ({"beautiful"}) );
    SetShort("a beautiful gold ring");
    SetLong("This is a simple but beautiful ring made of "+
      "gold, suitable for wearing on one's finger.");
    SetItems(([
	"inscription" : "This is an inscription on the ring. Try 'read inscription on ring'",
      ]));
    SetLanguage("edhellen");
    SetRead(([
	"inscription" : "So! We, the spear-Danes",
      ]));
    SetMass(1);
    SetDollarCost(5000);
    SetDamagePoints(100);
    SetArmorType(A_RING);
}
void init(){
    ::init();
}
