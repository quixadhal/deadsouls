#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_ARMOR;

object bearshadow;

static void create(){
    armor::create();
    SetKeyName("bearsuit");
    SetId(({"costume","bear costume","suit","bear suit"}));
    SetAdjectives(({"realistic","terrific"}));
    SetShort("a bear costume");
    SetLong("A terrific and very realistic suit which makes "+
            "the wearer look like a bear. It seems to have "+
            "been fashioned from real bear parts.");
    SetMass(200);
    SetMatching(0);
    SetBaseCost("silver",800);
    SetProtection(BLUNT,5);
    SetProtection(BLADE,10);
    SetProtection(KNIFE,10);
    SetArmorType(A_BODY_ARMOR);
}

void init(){
    ::init();
}

mixed eventEquip(object who, string array limbs){
    mixed success = armor::eventEquip(who, limbs);
    bearshadow = new("/shadows/bear");
    if(success){
        if(bearshadow) bearshadow->eventShadow(who);
    }
    else if(bearshadow) destruct(bearshadow);
    return success;
}

varargs mixed eventUnequip(object who) {
    mixed success;
    if(!who) who = this_player();
    success = armor::eventUnequip(who);
    if(success){
        if(bearshadow) who->unbearshadow();
    }
    return success;
}
