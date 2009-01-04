#include <lib.h>

inherit  LIB_TEACHER;
void create(){
    ::create();
    SetKeyName("bubb");
    SetId( ({"teacher","orc","schoolteacher","bubb the schoolteacher"}) );
    SetGender("male");
    SetRace("orc");
    SetNativeLanguage("english");
    SetTeachingFee(100);
    SetLocalCurrency("silver");
    SetShort("Bubb the schoolteacher");
    SetLong("This is a person whose job it is to teach you things. "+
            "For example, 'ask teacher "+
            "to teach Tangetto' would prompt him to begin teaching a Tangetto "+
            "lesson to you, if he knows the language and you have "+
            "enough of his preferred currency. ");
    SetLevel(1);
    SetLanguage("Tangetto", 100);
    AddTeachingLanguages( ({"Tangetto", "English" }) );
}
void init() {
    ::init();
}
