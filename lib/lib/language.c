/*    /lib/languages.c
 *    from the Dead Souls LPC Library
 *    language module for linguistic people
 *    created by Descartes of Borg 950412
 */

#include <function.h>

class comprehension {
    function check;
    int time;
    function end;
}

private mapping                    Languages      = ([]);
private static class comprehension Comprehension  = 0;

// abstract methods
int GetHeartRate();
int GetStatLevel(string stat);
// end abstract methods

int GetNextLevel(string lang, int curr_level);

varargs static void SetLanguage(string lang, int level, int native) {
    string key;

    key = convert_name(lang);
    if( !Languages[key] ) Languages[key] = ([]);
    Languages[key] = ([ "name" : lang, "points" : 0, "level" : level,
      "native" : native ]);
}

mapping RemoveLanguage(string lang) {
    lang = convert_name(lang);
    map_delete(Languages, lang);
    return Languages;
}

varargs void SetLanguageComprehension(function check, int time, function end) {
    if( !check ) {
	Comprehension = 0;
    }
    else {
	Comprehension = new(class comprehension);
	Comprehension->check = check;
	Comprehension->time = time;
	Comprehension->end = end;
    }
}

int GetLanguageLevel(string lang) {
    if( Comprehension ) {
	int fp = functionp(Comprehension->check);

	if( fp && !(fp & FP_OWNER_DESTED) ) {
	    return evaluate(Comprehension->check, this_object(), lang);
	}
    }
    lang = convert_name(lang);
    if( !Languages[lang] ) return 0;
    else return Languages[lang]["level"];
}

string GetLanguageName(string lang) {
    lang = convert_name(lang);
    if( !Languages[lang] ) return 0;
    else return Languages[lang]["name"];
}

int AddLanguagePoints(string lang, int points) {
    string key;
    int y;
 
    key = convert_name(lang);
    if( !Languages[key] ) SetLanguage(lang, 0, 0);
    Languages[key]["points"] += points;
    while( Languages[key]["points"] > 
	  (y = GetNextLevel(key, Languages[key]["level"])) ) {
	Languages[key]["points"] -= y;
	Languages[key]["level"]++;
    }
    while( Languages[key]["points"] < 0 ) {
	y = GetNextLevel(key, Languages[key]["level"] - 1);
	Languages[key]["points"] = y + Languages[key]["points"];
	Languages[key]["level"]--;
    }
    return Languages[key]["points"];
}

int GetLanguagePoints(string lang) {
    lang = convert_name(lang);
    if( !Languages[lang] ) return 0;
    else return Languages[lang]["points"];
}

string array GetLanguages() {
    return map(keys(Languages), (: Languages[$1]["name"] :));
}

int GetNextLevel(string lang, int curr_level) {
    int y;

    lang = lower_case(lang);
    if( !Languages[lang] ) return 0;
    y = (1 + Languages[lang]["native"]) * GetStatLevel("intelligence");
    y = (2000 * curr_level) / y;
    return y;
}

int GetStatLevel(string stat) { return 0; }

string GetNativeLanguage() {
    mapping val;
    string lang;

    foreach(lang, val in Languages)
      if( val["native"] ) return Languages[lang]["name"];
}

static void heart_beat() {
    if( Comprehension ) {
	Comprehension->time -= GetHeartRate();
	if( Comprehension->time < 1 ) {
	    function tmp = Comprehension->end;

	    Comprehension = 0;
	    if( functionp(tmp) && !(functionp(tmp) & FP_OWNER_DESTED) ) {
		evaluate(tmp, this_object());
	    }
	}
    }
}
