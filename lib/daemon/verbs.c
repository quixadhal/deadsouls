/*    /daemon/verbs.c
 *    from the Dead Soulsr2 Object Library
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
    eventReloadVerbs();
}

varargs void eventReloadVerbs(mixed val) {
    string *verbs = 0;
    string verb;
    int i;

    if( arrayp(val) ) verbs = filter(val, (: GetValidVerb($1) :));
    else if( stringp(val) ) {
	if( strlen(val) > 2 && val[<2..] == ".c" ) val = val[0..<3];
	if( file_exists(val + ".c") && GetValidVerb(val) ) verbs = ({ val });
	else {
	    string cwd, tmp;

	    if( this_player() && cwd = (string)this_player()->query_cwd() ) {
		tmp = absolute_path(cwd, val);
		if( file_exists(tmp + ".c") && GetValidVerb(tmp)  ) 
		  verbs = ({ tmp });
	    }
	    if( !verbs ) {
		if( file_size(tmp) == -2 && GetValidVerb(tmp) ) 
		  verbs = map(get_dir(tmp+"/*.c"), (: $(tmp) + "/" + $1 :));
	    }
	    else {
		string dir;

		foreach(dir in get_dir(DIR_VERBS + "/")) {
		    dir = DIR_VERBS + "/" + dir;
		    if( file_size(dir) != -2 ) continue;
		    if( file_exists( dir + "/" + val + ".c") ) {
			verbs = ({ dir + "/" + val });
			break;
		    }
		}
		if( !verbs ) return;
	    }
	}
    }
    else {
	string dir;

	verbs = ({});
	foreach(dir in get_dir(DIR_VERBS + "/")) { 
	    dir = DIR_VERBS + "/" + dir;
	    if( file_size(dir) == -2 ) 
	      verbs += map(get_dir(dir + "/*.c"), (: $(dir) + "/" + $1 :));
	}
    }
    i = 0;
    foreach(verb in verbs) {
	object ob;
	string *verb_list;
	
	if( ob = find_object(verb) ) ob->eventDestruct();
	if( ob = load_object(verb) ) {
	    i++;
	    if( !(verb_list = (string *)ob->GetVerbs()) ) 
	      verb_list = ({ explode(verb, "/")[<1][0..<3] });
	    else {
		verb_list += (string *)ob->GetSynonyms();
		verb_list =distinct_array(map(verb_list,
					      (: explode($1," ")[0] :)));
	    }
	    Verbs += expand_keys(([ verb_list : verb ]));
	}
    }
    return;
}

string GetErrorMessage(string verb) {
    if( !Verbs[verb] ) return 0;
    else return (string)Verbs[verb]->GetErrorMessage();
}

int GetValidVerb(string verb) {
    return !strsrch(verb, DIR_VERBS);
}

mapping GetVerbs() { return copy(Verbs); }



