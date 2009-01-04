#include <lib.h>
#include <vendor_types.h>
inherit LIB_ITEM;
inherit LIB_READ;

string ReadFun(){
    string ret = "Commander Kurogane,\n";
    ret += "We cannot assist you at this time. Azusa coast is "+
        "under concentrated attack by an invasion force. All our "+
        "soldiers are needed here. You are ordered to maintain your "+
        "garrison in place and continue to observe and report on "+
        "humanoid activity at the eastern frontiers. Notify us "+
        "at once if you observe unusual lights in the sky.\n\n"+
        "-General Ikomo"; 
    return ret;
}

void create(){
    ::create();
    SetKeyName("letter");
    SetId(({"letter","sheet","paper","sheet of paper"}));
    SetAdjectives(({"small"}));
    SetShort("a letter");
    SetLong("This is a sheet of paper with a message printed on it. ");
    SetMass(1);
    AddItem("message","A message written on paper.");
    SetRead("default", (: ReadFun :) );
    SetRead(({"message"}), (: ReadFun :) );
    SetLanguage("Tangetto");
}

void init(){
    ::init();
}
