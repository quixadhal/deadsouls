#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit "/domains/default/armor/breather";

string LongD(){
    string ret = "This remarkably small device fits over the "+
        "wearer's mouth, and provides a long supply of oxygenated "+
        "air. Its current charge level is "+
        to_int(percent(charge,maxcharge))+" percent.";
    return ret;
}

static void create(){
    ::create();
    SetKeyName("breathing mask");
    SetId(({"mask","breather","a98","apparatus","device"}));
    SetAdjectives(({"a98","breathing"}));
    SetShort("an A98 breathing device");
    SetLong( (: LongD :) );
    SetMass(30);
    SetBaseCost("silver",900);
    SetArmorType(A_CUSTOM);
    SetRestrictLimbs( ({ "head"}) );
    AddSave( ({ "charge" }) );
    SetCharge(100);
    SetMaxCharge(100);
}

void init(){
    ::init();
}
