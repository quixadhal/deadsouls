#include <lib.h>
#include <vendor_types.h>
inherit LIB_BOOK;

void create(){
    ::create();
    SetKeyName("players handbook");
    SetAdjectives( ({"player", "players"}) );
    SetId( ({"handbook", "book", "hbook"}) );
    SetShort("a Players Handbook");
    SetLong("This is a handbook for players. To read it, you must read the chapters in it. For example, 'read chapter 1 in handbook', or 'read index in handbook'");
    SetProperties(([
	"no steal" : 1,
      ]));
    SetMass(3);
    SetBaseCost("silver",10);
    SetVendorType(VT_TREASURE);
    SetTitle("The Players Handbook");
    SetSource("/doc/hbook");
    SetDefaultRead("Try \"read chapter 1 in handbook\" or "
      "\"read index in handbook\"");

}
void init(){
    ::init();
}
