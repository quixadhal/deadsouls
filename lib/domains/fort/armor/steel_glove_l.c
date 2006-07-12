#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_ARMOR;
static void create(){
    armor::create();
    SetKeyName("steel glove");
    SetId(({"glove","steel glove"}));
    SetAdjectives( ({"round","shiny","strong","light","left","left hand"}) );
    SetShort("a steel glove");
    SetLong("This is the steel battle glove of a professional soldier. "+
      "It is very strong and relatively light. The outside is covered in "+
      "articulated plating which allows for great protection and good "+
      "dexterity. It is designed for wear on the left hand.");
    SetMass(50);
    SetDollarCost(100);
    SetDamagePoints(10);
    SetProtection(BLUNT,10);
    SetProtection(BLADE,10);
    SetProtection(KNIFE,10);
    SetArmorType(A_GLOVE);
    SetRestrictLimbs(({"left hand"}));
}
