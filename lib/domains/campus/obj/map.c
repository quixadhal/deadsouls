#include <lib.h>
#include <vendor_types.h>
inherit LIB_ITEM;
inherit LIB_READ;


int TestFunc(){
    this_player()->eventPage("/domains/campus/txt/map.txt","system");
    return 1;
}
void create(){
    ::create();
    SetKeyName("building map");
    SetId(({"map","map of the administrative building","small map of the administrative building"}));
    SetAdjectives(({"small"}));
    SetShort("a small map of the administrative building");
    SetLong("A map of the administrative building. Try: read map \n");
    SetMass(0);
    SetDollarCost(0);
    SetVendorType(VT_TREASURE);
    SetRead((: TestFunc :));
}
void init(){
    ::init();
}
