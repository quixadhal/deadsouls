#include <lib.h>
#include <objects.h>

inherit  LIB_CLERK;
void create(){
    ::create();
    SetKeyName("clerk");
    SetId("clerk");
    SetAdjectives("town");
    SetGender("male");
    SetRace("human");
    SetShort("the town clerk");
    SetLong("An officious-looking clerk."); 
    SetLocalCurrency("silver");
    SetClass("priest");
    SetLevel(4);
    SetTax(5);
}
void init() {
    ::init();
    SetSmell(([  "default" : "A rather odd, musty smell."]));
    SetListen(([  "default" : "The clerk grumbles something about an "+
	"unconscionably small tax revenue base."]));
}
