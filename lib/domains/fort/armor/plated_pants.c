#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_ARMOR;
static void create(){
    armor::create();
    SetKeyName("platemail pants");
    SetId(({"pants","platemail pants"}));
    SetAdjectives( ({"platemail","steel"}) );
    SetShort("a platemail pants");
    SetLong("These are leather pants covered in chainmail, with "+
      "metal plates covering the shins, thighs, and knees. They "+
      "don't look comfortable, but they provide exceptional protection "+
      "and mobility.");
    SetMass(200);
    SetDollarCost(5000);
    SetDamagePoints(100);
    SetProtection(BLUNT,20);
    SetProtection(BLADE,25);
    SetProtection(KNIFE,25);
    SetArmorType(A_PANTS);
}
