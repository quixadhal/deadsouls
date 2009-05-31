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
    SetLong("An officious-looking clerk. He is empowered to perform ceremonies of public record. You may \"ask clerk to marry me and PERSON\", and \"ask clerk for a divorce\".");
    SetLocalCurrency("silver");
    SetClass("cleric");
    SetLevel(4);
    SetTax(5);
    SetPolyglot(1);
    SetLanguage("common", 100);
    SetDefaultLanguage("common");
}
void init() {
    ::init();
    SetSmell(([  "default" : "A rather odd, musty smell."]));
    SetListen(([  "default" : "The clerk grumbles something about an "+
                "unconscionably small tax revenue base."]));
}
