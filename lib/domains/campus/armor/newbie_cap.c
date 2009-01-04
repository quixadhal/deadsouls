#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_ARMOR;
//inherit "/lib/events/read";

string eventReadBill(){
    return "The front of the cap reads: \"Kiss me, I'm a newbie!\"";
}
static void create(){
    armor::create();
    SetKeyName("baseball cap");
    SetId(({"cap","hat"}));
    SetAdjectives(({"baseball"}));
    SetShort("a baseball cap");
    SetLong("This baseball cap looks pretty normal, except "+
            "the trim is fine corinthian leather and the bill "+
            "is plated in what appears to be pure mithril. There is some "+
            "writing on the front of the cap.");
    SetMass(50);
    SetDollarCost(100);
    SetDamagePoints(20);
    SetArmorType(A_HELMET);
    SetProtection(BLUNT, 20);
    SetProtection(BLADE, 20);
    SetProtection(KNIFE, 20);
    SetItems( ([
                ({"brim","bill","front"}) : "For some reason the front of this cap "+
                "is plated with a fine layer of mithril...one of the strongest "+
                "and most expensive substances known to man.",
                ({"writing","letters"}): "Something is written on the front of the "+
                "cap. Perhaps you could \"read writing on cap\"",
                ]) );

    SetRead( ([
                ({"front","writing","letters"}) : "The front of the cap reads: \"Kiss me, I'm a newbie!\"",
                ]) );
    SetDefaultRead("default", "The front of the cap reads: \"Kiss me, I'm a newbie!\"");
}
