/*    /lib/search.c
 *    from the Dead Soulsr2 Object Library
 *    allows people to search things
 *    created by Descartes of Borg 951008
 *    Version: @(#) search.c 1.7@(#)
 *    Last modified: 97/01/01
 */

#include <function.h>

static private mixed Search     = 0;
static private mapping Searches = ([]);

// abstract methods
string GetShort();
// end abstract methods

varargs string GetSearch(string str, object who) {
    mixed val;
    
    if( !str || str == "default" ) {
	val = Search;
    }
    else {
	val = Searches[str];
    }
    if( !val ) {
	return 0;
    }
    if( functionp(val) ) {
	if( functionp(val) & FP_OWNER_DESTED ) {
	    return "An error occured in a function pointer.";
	}
	return evaluate(val, who, str);
    }
    else if( arrayp(val) ) {
	return val[query_night()];
    }
    else return val;
}

string array GetSearches() {
    return keys(Searches);
}

mapping RemoveSearch(string item) {
    if( !item || item == "default" ) {
	Search = 0;
    }
    else {
	map_delete(Searches, item);
    }
    return Searches;
}

varargs mixed SetSearch(mixed array args...) {
    if( sizeof(args) == 1 ) {
	if( mapp(args[0]) ) {
	    if( args[0]["default"] ) {
		Search = args[0]["default"];
		map_delete(args[0], "default");
	    }
	    return (Searches = expand_keys(args[0]));
	}
	else {
	    Search = args[0];
	}
	return args[0];
    }
    else if( sizeof(args) == 2 ) {
	if( !args[1] ) {
	    return SetSearch(args[0]);
	}
	else if( arrayp(args[0]) ) {
	    foreach(string item in args[0]) {
		SetSearch(item, args[1]);
	    }
	    return args[1];
	}
	else {
	    if( !args[0] || args[0] == "default" ) {
		Search = args[1];
		return Search;
	    }
	    else {
		Searches[args[0]] = args[1];
		return Searches[args[0]];
	    }
	}
    }
    else {
	error("Wrong number of arguments to SetSearch():\n\t"
	      "Expected 1 or 2, got " + sizeof(args) + "\n");
    }
}

varargs mixed eventSearch(object who, string str) {
    str = GetSearch(str, who);
    if( !str ) {
	who->eventPrint("There is nothing to search.");
	return 1;
    }
    environment(who)->eventPrint(who->GetName() + " searches " + GetShort() +
				 ".", who);
    who->eventPrint(str);
    return 1;
}
		   
mixed direct_search_obj() {
    if( !Search ) {
	return 0;
    }
    else {
	return 1;
    }
}

mixed direct_search_str_word_obj(string str) {
    str = remove_article(lower_case(str));
    if( !Searches[str] ) {
	return 0;
    }
    else {
	return 1;
    }
}

