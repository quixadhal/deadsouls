/*    /lib/languages.c
 *    from the Dead Souls LPC Library
 *    language module for linguistic people
 *    created by Descartes of Borg 950412
 */

#include <function.h>

int Polyglot = 0;

private mapping                    Languages      = ([]);
private string                     DefaultLanguage = "";

// abstract methods
int GetHeartRate();
int GetStatLevel(string stat);
string GetNativeLanguage();
// end abstract methods

int GetNextLevel(string lang, int curr_level);

varargs void SetLanguage(string lang, int level, int native){
    string key;
    if(!lang || !sizeof(lang)) return;
    key = convert_name(lang);
    if( !Languages[key] ) Languages[key] = ([]);
    Languages[key] = ([ "name" : lang, "points" : 0, "level" : level,
            "native" : native ]);
}

mapping RemoveLanguage(string lang){
    if(lang && sizeof(lang)) lang = convert_name(lang);
    else return copy(Languages);
    map_delete(Languages, lang);
    return copy(Languages);
}

mapping SetNativeLanguage(string lang){
    string old_lang = GetNativeLanguage();
    RemoveLanguage(old_lang);
    SetLanguage(old_lang, 100);
    SetLanguage(lang, 100, 1);
    return copy(Languages);
}

mixed SetDefaultLanguage(string str){
    if(!str || str == "") return DefaultLanguage = GetNativeLanguage();
    if(member_array(lower_case(str),keys(Languages)) != -1 ||
            member_array(capitalize(lower_case(str)),keys(Languages)) != -1)
        return DefaultLanguage = capitalize(lower_case(str));
    else return DefaultLanguage = GetNativeLanguage();
}

string GetDefaultLanguage(){
    if(sizeof(DefaultLanguage)) return DefaultLanguage;
    else return GetNativeLanguage();
}

int GetLanguageLevel(string lang){
    if(Polyglot) return 100;
    lang = convert_name(lang);
    if( !Languages[lang] ) return 0;
    else return Languages[lang]["level"];
}

string GetLanguageName(string lang){
    lang = convert_name(lang);
    if( !Languages[lang] ) return 0;
    else return Languages[lang]["name"];
}

int AddLanguagePoints(string lang, int points){
    string key;

    key = convert_name(lang);
    if( !Languages[key] ) SetLanguage(key, 0, 0);
    Languages[key]["points"] += points;
    Languages[key]["level"] = Languages[key]["points"];
    if(Languages[key]["level"] > 100) Languages[key]["level"] = 100;
    return Languages[key]["points"];
}

int GetLanguagePoints(string lang){
    lang = convert_name(lang);
    if( !Languages[lang] ) return 0;
    else return Languages[lang]["points"];
}

string array GetLanguages(){
    return map(keys(Languages), (: Languages[$1]["name"] :));
}

int GetNextLevel(string lang, int curr_level){
    int y;

    lang = lower_case(lang);
    if( !Languages[lang] ) return 0;
    y = (1 + Languages[lang]["native"]) * GetStatLevel("intelligence");
    y = (2000 * curr_level) / y;
    return y;
}

int GetStatLevel(string stat){ return 0; }

string GetNativeLanguage(){
    mapping val;
    string lang;

    foreach(lang, val in Languages)
        if( val["native"] ) return Languages[lang]["name"];
}

int SetPolyglot(int i){
    if(!i) Polyglot = 0;
    else Polyglot = 1;
    return Polyglot;
}

int GetPolyglot(){
    return Polyglot;
}
