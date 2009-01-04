#include <lib.h>
#include <vendor_types.h>
inherit LIB_ITEM;
inherit LIB_READ;

int ReadFunc(){
    this_player()->eventPage("/domains/town/txt/map.txt","system");
    return 1;
}

void create(){
    ::create();
    SetKeyName("town map");
    SetId(({"map","map of the town"}));
    SetAdjectives(({"small"}));
    SetShort("a small map of the town");
    SetLong("A map of the town. Try: read map \n");
    SetMass(1);
    SetDollarCost(0);
    SetVendorType(VT_TREASURE);
    SetRead((: ReadFunc :));
}

void init(){
    ::init();
}
