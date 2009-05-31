#include <lib.h>
#include <vendor_types.h>
inherit LIB_ITEM;
inherit LIB_QUEST;

void create(){
    item::create();
    quest::create();
    SetKeyName("generic quest object");
    SetId( ({"thing","item","thang","dingus", "object"}) );
    SetAdjectives( ({"generic","sample","template","quest"}) );
    SetShort("a generic quest object");
    SetLong("This is a relic of an older time, and here for "+
            "compatibility purposes.");
    SetMass(20);
    SetBaseCost("silver",10);
    SetVendorType(VT_TREASURE);
    SetQuestPoints(1);
}
void init(){
    ::init();
}
