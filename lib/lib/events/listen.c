/*    /lib/events/listen.c
 *    From the Dead Souls Object Library
 *    Handles responses to the listen verb
 *    Created by Descartes of Borg 951008
 *    Version: @(#) listen.c 1.8@(#)
 *    Last modified: 96/12/31
 */

#include <function.h>

static private mixed   Listen  = 0;
static private mapping Listens = ([]);

// abstract methods
string GetShort();
// end abstract methods

varargs string GetListen(string str, object who) {
    mixed val;
    
    if( !str || str == "default" ) {
	val = Listen;
    }
    else {
	val = Listens[str];
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

string array GetListens() {
    return keys(Listens);
}

mapping RemoveListen(string item) {
    if( !item || item == "default" ) {
	Listen = 0;
    }
    else {
	map_delete(Listens, item);
    }
    return Listens;
}

varargs mixed SetListen(mixed array args...) {
    if( sizeof(args) == 1 ) {
	if( mapp(args[0]) ) {
	    if( args[0]["default"] ) {
		Listen = args[0]["default"];
		map_delete(args[0], "default");
	    }
	    return (Listens = expand_keys(args[0]));
	}
	else {
	    Listen = args[0];
	}
	return args[0];
    }
    else if( sizeof(args) == 2 ) {
	if( !args[1] ) {
	    return SetListen(args[0]);
	}
	else if( arrayp(args[0]) ) {
	    foreach(string item in args[0]) {
		SetListen(item, args[1]);
	    }
	    return args[1];
	}
	else {
	    if( !args[0] || args[0] == "default" ) {
		Listen = args[1];
		return Listen;
	    }
	    else {
		Listens[args[0]] = args[1];
		return Listens[args[0]];
	    }
	}
    }
    else {
	error("Wrong number of arguments to SetListen():\n\t"
	      "Expected 1 or 2, got " + sizeof(args) + "\n");
    }
}

varargs mixed eventListen(object who, string str) {
    str = GetListen(str, who);
    if( !str ) {
	who->eventPrint("You hear nothing unusual.");
	return 1;
    }
    environment(who)->eventPrint(who->GetName() + " listens to " + GetShort() +
				 ".", who);
    who->eventPrint(str);
    return 1;
}
		   
mixed direct_listen_obj() {
    if( !Listen ) {
	return "You hear nothing unusual.";
    }
    else {
	return 1;
    }
}

mixed direct_listen_to_obj() {
    return direct_listen_obj();
}

mixed direct_listen_to_str_word_obj(string str) {
    str = remove_article(lower_case(str));
    if( !Listens[str] ) {
	return "You hear nothing special.";
    }
    else {
	return 1;
    }
}
