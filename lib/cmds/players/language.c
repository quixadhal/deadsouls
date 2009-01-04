#include <lib.h>

inherit LIB_DAEMON;

mixed cmd(string str) {
    mapping FluencyMap = ([]);
    string *langs = this_player()->GetLanguages();

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

    return 1;
}

void help() {
    message("help",
            "Syntax: language\n\n"
            "This command reports which languages you speak and understand.\n\n",
            this_player() );
}

