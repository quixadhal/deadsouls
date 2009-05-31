#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_ARMOR;
static void create(){
    armor::create();
    SetKeyName("silver_ring");
    SetId(({"ring","silver ring"}));
    SetAdjectives( ({"beautiful"}) );
    SetShort("a beautiful silver ring");
    SetLong("This is a simple but beautiful ring made of "+
            "silver, suitable for wearing on one's finger. The ring bears an inscription.");
    SetMass(1);
    SetDollarCost(5000);
    SetDamagePoints(100);
    SetArmorType(A_RING);
    SetItems(([
                ({"script","words","inscription","writing"}) : "Words engraved on the ring in a highly "+
                "stylized script."
                ]) );
    SetRead(({"script","words","inscription","writing"}), "\"etaoin shrdlu\""  );

}
void init(){
    ::init();
}
