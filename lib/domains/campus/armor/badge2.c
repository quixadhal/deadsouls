#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_ARMOR;

static void create(){
    armor::create();
    SetKeyName("identification badge");
    SetId(({"badge","pass","id","i.d.","pass of dr kleiner"}));
    SetAdjectives( ({"clip on", "plastic", "id","i.d.","access","kleiner"}) );
    SetShort("an identification badge");
    SetLong("This clip-on plastic badge grants the owner access to "+
            "restricted areas. There is a photo of someone on it, as well "+
            "as some writing.");
    SetMass(10);
    SetDollarCost(5);
    SetDamagePoints(100);
    SetArmorType(A_AMULET);
    SetRestrictLimbs( ({ "torso" }) );
    SetReads( ([
                "default" : "Isaac Kleiner, Ph.D.",
                "writing" : "Isaac Kleiner, Ph.D.",
                ]) );
    SetItems( ([
                ({"photo","picture","photograph"}) : "An older, white-haired "+
                "gentleman, wearing an affable, goofy grin.",
                ]) );
}
void init(){
    ::init();
}
