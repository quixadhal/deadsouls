#include <lib.h>
#include <vendor_types.h>
inherit LIB_BOOK;

void create(){
    ::create();
    SetKeyName("hintbook");
    SetId( ({"book", "hints", "book of hints"}) );
    SetAdjectives( ({"hint", "player", "players"}) );
    SetShort("a Player's Hint Book");
    SetLong("This is a book of hints. To read it, you must read the chapters in it. For example, 'read chapter 1 in hintbook', or 'read index in hintbook'");
    SetProperties(([
                "no steal" : 1,
                ]));
    SetMass(3);
    SetBaseCost("silver",10);
    SetVendorType(VT_TREASURE);
    SetTitle("The Player's Hint Book");
    SetSource("/doc/phints");
    SetDefaultRead("Try \"read chapter 1 in hintbook\" or "
            "\"read index in hintbook\"");
}

void init(){
    ::init();
}
