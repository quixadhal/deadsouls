#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_ARMOR;

static void create(){
    armor::create();
    SetKeyName("bearsuit");
    SetId(({"costume","bear costume","suit","bear suit"}));
    SetAdjectives(({"realistic","terrific"}));
    SetShort("a bear costume");
    SetLong("A terrific and very realistic suit which makes "+
      "the wearer look like a bear. It seems to have "+
      "been fashioned from real bear parts.");
    SetMass(500);
    SetMatching(0);
    SetBaseCost("silver",1000);
    SetDamagePoints(10);
    SetProtection(BLUNT,10);
    SetProtection(BLADE,20);
    SetProtection(KNIFE,20);
    SetArmorType(A_BODY_ARMOR);
}
void init(){
    ::init();
}
mixed eventEquip(object who, string array limbs){
    mixed success = armor::eventEquip(who, limbs);
    object bearshadow = new("/shadows/bear");
    if(success){
        tc("success");
        bearshadow->eventShadow(who);
    }
    else destruct(bearshadow);
    return success;
}

varargs mixed eventUnequip(object who) {
    mixed success;
    if(!who) who = this_player();
    success = armor::eventUnequip(who);
    if(success){
        who->unbearshadow();
    }
    return success;
}
