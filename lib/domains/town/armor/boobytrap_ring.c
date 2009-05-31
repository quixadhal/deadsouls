/*
 * Note that this ring, unlike the boobytrapped dagger, does
 * not check to see if it has been previously armed. This prevents
 * a player from disarming the ring by simply logging out
 * and logging back in.
 */

#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
#include <boobytraps.h>

inherit LIB_ARMOR;

void boobytrap_me(){
    object shadowtrap;
    if(!clonep(this_object())) return; /* Avoids boobytrapping the blueprint ob */

    shadowtrap = new("/shadows/needle_trap");
    shadowtrap->SetTrapDescription("A poison needle trap.");
    shadowtrap->SetTrapLevel(50);
    shadowtrap->SetAutoResets(0);
    shadowtrap->SetTrapType(BOOBYTRAP_WEAR);
    shadowtrap->eventShadow(this_object());
}

static void create(){
    armor::create();
    SetKeyName("silver_ring");
    SetId(({"ring","silver ring"}));
    SetAdjectives( ({"beautiful"}) );
    SetShort("a beautiful silver ring");
    SetLong("This is a simple but beautiful ring made of "+
            "silver, suitable for wearing on one's finger. The ring bears an inscription.");
    SetMass(1);
    SetBaseCost("silver",1000);
    SetArmorType(A_RING);
    SetItems(([
                ({"script","words","inscription","writing"}) : "Words engraved on the ring in a highly "+
                "stylized script."
                ]) );
    SetRead(({"script","words","inscription","writing"}), "\"etaoin shrdlu\""  );
    boobytrap_me();
}

void init(){
    ::init();
}
