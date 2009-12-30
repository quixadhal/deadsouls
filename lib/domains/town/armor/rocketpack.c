#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
#include <medium.h>
inherit "/domains/default/armor/rocketpack";


string LongD(){
    string ret = "This is a small device worn on the back and designed to \"boost\" " +
        "the wearer through the air or through space. For continuous operation, "+
        "it should probably be activated.  The fuel gauge "+
        "reads "+to_int(percent(charge,maxcharge))+" percent.";
    return ret;
}

static void create(){
    ::create();
    if(!Directions) Directions = ([]);
    SetKeyName("rocket pack");
    SetId(({"pack","rocket","rocketpack"}));
    SetAdjectives(({"small","rocket"}));
    SetShort("a small rocket pack");
    SetLong( (: LongD :) );
    SetMass(500);
    SetBaseCost(8000);
    SetArmorType(A_ARMOR);
    AddSave( ({ "charge", "Directions" }) );
    SetRestrictLimbs( ({ "torso" }) );
    SetCharge(200);
    SetMaxCharge(200);
}

void init(){
    ::init();
}
