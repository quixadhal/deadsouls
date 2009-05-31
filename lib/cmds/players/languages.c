#include <lib.h>

inherit LIB_DAEMON;

mixed cmd(string str) {
    mapping FluencyMap = ([]);
    string *langs = this_player()->GetLanguages();

    if(str){
        write("Try: help languages\nor: help language");
        return 1;
    }

    foreach(string lang in langs){
        FluencyMap[lang] = this_player()->GetLanguageLevel(lang);
    }

    if(this_player()->GetPolyglot()){
        write("You understand all languages with 100% proficiency.");
    }

    write("You speak: ");
    foreach(string key, int val in FluencyMap){
        write(capitalize(key)+" with "+val+"% proficiency.");
    }
    write("Your current default language is: "+
            this_player()->GetDefaultLanguage()+".");

    return 1;
}
