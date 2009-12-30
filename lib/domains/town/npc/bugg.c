#include <lib.h>

inherit  LIB_TEACHER;

void create(){
    ::create();
    SetKeyName("bugg");
    SetId( ({"teacher","dwarf","schoolteacher","bugg the schoolteacher"}) );
    SetGender("male");
    SetRace("dwarf");
    SetNativeLanguage("common");
    SetShort("Bugg the schoolteacher");
    SetLong("This is a person whose job it is to teach you things. "+
            "For example, 'ask teacher "+
            "to teach Malkierien' would prompt him to begin teaching a Malkierien "+
            "lesson to you, if he knows the language and you have "+
            "earned sufficient training points. If you lack training points, then "+
            "do some adventuring and earn a level promotion. You will "+
            "then be awarded training points. To know more about what Bugg "+
            "can teach: ask bugg for help"); 
    SetLevel(1);
    SetLanguage("Malkierien", 100);
    SetLanguage("English", 100);
    SetLanguage("Edhellen", 100);
    SetLanguage("Tangetto", 100);
    SetLanguage("Poleepkwa", 100);
    AddTeachingLanguages( ({"Malkierien", "English", "Common",
                "Edhellen", "Tangetto", "Poleepkwa" }) );
    SetPolyglot(1);
    SetLanguage("common", 100);
    SetDefaultLanguage("common");
}
void init() {
    ::init();
}
