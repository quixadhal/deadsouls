#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_ARMOR;

static void create(){
    armor::create();
    SetKeyName("suit of armor");
    SetId(({"armor","plate mail","platemail","suit","armor","suit of armor"}));
    SetAdjectives(({"full","plate","plated","metal"}));
    SetShort("a full suit of armor");
    SetLong("This is a large, very heavy suit of armor, the kind "+
            "one might expect a knight to wear. The thick "+
            "metal plating promises extraordinary protection, "+
            "yet the joints are so finely articulated as to "+
            "permit great freedom of movement.");
    SetMass(900);
    SetBaseCost("silver",10000);
    SetDamagePoints(100);
    SetProtection(BLUNT,40);
    SetProtection(BLADE,60);
    SetProtection(KNIFE,60);
    SetProtection(PIERCE,60);
    SetArmorType(A_BODY_ARMOR);
}

void init(){
    ::init();
}
