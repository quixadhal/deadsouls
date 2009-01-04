#include <lib.h>
#include <vendor_types.h>
inherit LIB_ITEM;
inherit LIB_READ;

int TestFunc(object ob,string str){
    if(ob) ob->eventPrint("if you are seeing this, you may be ob.");
    this_player()->eventPrint("wtf");
    if(str && str !="") write(str);
    if(!str) write("Null string found.");
    if(str =="") write("Empty string found.");
    write("WTF");
    return 1;
}

void create(){
    ::create();
    SetKeyName("leaflet");
    SetId(({"leaflet","paper","slip"}));
    SetAdjectives(({"small"}));
    SetShort("a small leaflet");
    SetLong("This is a single sheet of paper with a message printed on it. ");
    SetMass(1);
    SetDollarCost(1);
    SetVendorType(VT_TREASURE);
    SetRead((: TestFunc :));
}

