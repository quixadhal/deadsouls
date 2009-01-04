#include <lib.h>
inherit  LIB_MAYOR;
void create(){
    ::create();
    SetKeyName("mayor");
    SetId("mayor");
    SetGender("male");
    SetRace("human");
    SetShort("the mayor");
    SetLong("A bit disheveled, this rather eccentric-looking "+
            "man discharges the official duties of this town's "+
            "chief executive office. Mostly, people request "+
            "citizenship from him, for a fee of 5 silver.");
    SetLocalCurrency("silver");
    SetTax(5);
}
//funs
//snuf
void init() {
    ::init();
    SetSmell(([  "default" : "A rather odd, musty smell."]));
    SetListen(([  "default" : "The mayor grumbles something about an "+
                "unconscionably small tax revenue base."]));
}
