#include <lib.h>
#include <vendor_types.h>
inherit LIB_BOOK;

void create(){
    ::create();
    SetKeyName("creator's manual");
    SetId( ({"manual", "book", "creators manual"}) );
    SetAdjectives( ({"creator", "creators", "coder", "coders"}) );
    SetShort("a Creator's Manual");
    SetLong("This is a manual for creators and admins. "
            "To read it, you must read the chapters in it. For example, \"read "
            "chapter 1 in manual\".\n If you are new to coding, you should "
            "start with chapter 31.");
    SetNoCondition(1);
    SetProperties(([
                "no steal" : 1,
                ]));
    SetMass(20);
    SetBaseCost("silver",10);
    SetVendorType(VT_TREASURE);
    SetTitle("The Coder's Manual");
    SetSource("/doc/manual");
    SetDefaultRead("Try \"read chapter 1 in manual\" or "
            "\"read index in manual\"\n"
            "New creators should especially read the Quick Creation "
            "System documentation, starting in chapter 31");
}
void init(){
    ::init();
}
