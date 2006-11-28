#include <lib.h>
#include <vendor_types.h>
inherit LIB_ITEM;
inherit LIB_READ;

int TestFunc(){
    this_player()->eventPage("/domains/campus/txt/note.txt","system");
    return 1;
}


void create(){
    ::create();
    SetKeyName("note");
    SetId(({"paper","slip"}));
    SetAdjectives(({"small"}));
    SetShort("a note");
    SetLong("A note with writing on it. Try: read note \n");
    SetMass(0);
    SetDollarCost(0);
    SetVendorType(VT_TREASURE);
    SetRead((: TestFunc :));
}

