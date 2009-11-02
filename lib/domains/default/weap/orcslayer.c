#include <lib.h>
#include <damage_types.h>
#include <vendor_types.h>

inherit LIB_ITEM;
inherit LIB_READ;

static void create() {
    item::create();
    SetKeyName("Orc Slayer");
    SetId( ({ "orcslayer", "orc slayer","sword", "short sword", "shortsword"}));
    SetAdjectives( ({ "crafted","finely crafted","short","sharp","fine"}));
    SetShort("a finely crafted short sword");
    SetLong("This is a very fine blade, overed with ancient runes. Engraved on it is a picture of the sword slicing an orc.");
    SetMass(300);
    SetVendorType(VT_WEAPON);
    SetClass(150);
    SetDamageType(BLADE);
    SetWeaponType("blade");
    SetItems( ([
                ({"rune","runes"}) : "The runes' meaning is undecipherable to you.",
                ({"picture","engraving"}) : "An almost comical image of a startled orc sliced in half.",
                ]) );
    SetRead( ([
                ({"rune","runes"}) : "You do not understand them."
                ]) );
}

int eventStrike(object target) {
    if( target->GetRace() != "orc" ) return item::eventStrike(target);
    message("environment", "The orc slayer sword glows blue and emits a ghastly shrieking sound!",
            environment(target));
    return item::eventStrike(target) + random(50)+10;
}
