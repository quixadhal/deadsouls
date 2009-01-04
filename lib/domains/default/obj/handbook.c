#include <lib.h>
#include <vendor_types.h>
inherit LIB_BOOK;

void create(){
    ::create();
    SetKeyName("player's handbook");
    SetId( ({"book", "handbook", "players handbook"}) );
    SetAdjectives( ({"player", "players"}) );
    SetShort("a Player's Handbook");
    SetLong("This is a handbook for players. To read it, you must read the chapters in it. For example, 'read chapter 1 in handbook', or 'read index in handbook'");
    SetProperties(([
                "no steal" : 1,
                ]));
    SetMass(3);
    SetBaseCost("silver",10);
    SetVendorType(VT_TREASURE);
    SetTitle("The Player's Handbook");
    SetSource("/doc/hbook");
    SetDefaultRead("Try \"read chapter 1 in handbook\" or "
            "\"read index in handbook\"");

}
void init(){
    ::init();
}
