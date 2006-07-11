#include <lib.h>
#include <vendor_types.h>
inherit LIB_ITEM;
inherit LIB_READ;

int TestFunc(){
    this_player()->eventPage("/domains/campus/txt/list.txt","system");
    return 1;
}


void create(){
    ::create();
    SetKeyName("list");
    SetId(({"paper","slip"}));
    SetAdjectives(({"small"}));
    SetShort("a list");
    SetLong("A list on a slip of paper. Try: read list \n");
    SetMass(0);
    SetDollarCost(0);
    SetVendorType(VT_TREASURE);
    SetRead((: TestFunc :));
}

