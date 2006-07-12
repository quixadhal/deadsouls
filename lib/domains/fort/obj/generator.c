#include <lib.h>
#include <vendor_types.h>
inherit LIB_ITEM;

void create(){
    ::create();
    SetKeyName("generator");
    SetId( ({"gen"}) );
    SetAdjectives( ({"generic","sample","template"}) );
    SetShort("a beep generator.");
    SetLong("This generates beeps under specific and rather mysterious circumstances.");
    SetMass(20);
    SetBaseCost("silver",10);
    SetVendorType(VT_TREASURE);
}
void init(){
    ::init();
}

int eventBeep(){
    tc("BEEP!");
    return 1;
}
