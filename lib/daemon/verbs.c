/*    /daemon/verbs.c
 *    from the Dead Souls Object Library
 *    handles the loading and rehashing of verbs
 *    created by Descartes of Borg 951016
 */

#include <lib.h>
#include "include/verbs.h"

inherit LIB_DAEMON;

private mapping Verbs;

static void create() {
    daemon::create();
    Verbs = ([]);
    SetNoClean(1);
    eventReloadVerbs();
}

static int ScheduledVerbLoad(string *cache){
    foreach(string verb in cache){
        object ob;
        string *verb_list;
        if( ob = find_object(verb) ) catch(ob->eventDestruct());
        if( !catch(ob = load_object(verb)) && ob ) {
            if( !(verb_list = ob->GetVerbs()) )
                verb_list = ({ explode(verb, "/")[<1][0..<3] });
            else if(verb_list && ob->GetSynonyms()) {
                verb_list += ob->GetSynonyms();
            }
            Verbs += expand_keys(([ verb_list : verb ]));
        }
    }
}

varargs void eventReloadVerbs(mixed val) {
    string *cache, *verbs = 0;
    string verb;

    if( arrayp(val) ) verbs = filter(val, (: GetValidVerb($1) :));
    else if( stringp(val) ) {
        if( strlen(val) > 2 && val[<2..] == ".c" ) val = val[0..<3];
        if( GetValidVerb(val) ) verbs = ({ val });
        if( !verbs ) return;
    }
    else {
        string dir;
        verbs = ({});
        foreach(dir in get_dir(DIR_VERBS + "/")) { 
            dir = DIR_VERBS + "/" + dir;
            if( file_size(dir) == -2 ) 
                verbs += map(get_dir(dir + "/*.c"), (: $(dir) + "/" + $1 :));
        }
        foreach(dir in get_dir(DIR_SECURE_VERBS + "/")) {
            dir = DIR_SECURE_VERBS + "/" + dir;
            if( file_size(dir) == -2 )
                verbs += map(get_dir(dir + "/*.c"), (: $(dir) + "/" + $1 :));
        }
    }
    cache = ({});
    foreach(verb in verbs){
        object ob;
        string *verb_list;
        cache += ({ verb });
        verbs -= ({ verb });
        if(sizeof(cache) > 9 || !sizeof(verbs)){
            call_out("ScheduledVerbLoad", 1, copy(cache));
            cache = ({});
        }
    }
    return;
}

string GetErrorMessage(string verb) {
    if( !Verbs[verb] ) return 0;
    else return Verbs[verb]->GetErrorMessage();
}

int GetValidVerb(string verb) {
    if(!strsrch(verb, DIR_VERBS) || !strsrch(verb, DIR_VERBS)) return 1;
    else return 0;
}

mapping GetVerbs() { return copy(Verbs); }
mixed GetVerb(string str) { return Verbs[str]; }
