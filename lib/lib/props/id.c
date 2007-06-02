/*    /lib/props/id.c
 *    From the Dead Souls Object Library
 *    Attributes which identify objects
 *    Created by Descartes of Borg 961222
 *    Version: @(#) id.c 1.2@(#)
 *    Last modified: 96/12/22
 */

#include <lib.h>

private static string array Adjectives   = ({});
private string              CapName      = 0;
private static string array Id           = ({});
private static string array CanonicalId  = ({});
private static string array ExcludedIds  = ({});
private static string       KeyName      = 0;
private static object array NotifiedObjects = ({});

string GetKeyName();

string array GetAdjectives() {
    return Adjectives;
}

varargs string array SetAdjectives(mixed adjs...) {
    if( stringp(adjs) ) {
	adjs = ({ adjs });
    }
    else if( !arrayp(adjs) ) {
	error("Bad argument 1 to SetAdjectives().\n");
    }
    Adjectives = ({});
    foreach(mixed val in adjs) {
	if( arrayp(val) ) {
	    Adjectives += val;
	}
	else {
	    Adjectives += ({ val });
	}
    }
    parse_refresh();
    return Adjectives;
}

string GetCapName() {
    return CapName;
}

string SetCapName(string str) {
    if( !stringp(str) ) {
	error("Bad argument 1 to SetCapName().\n");
    }
    return (CapName = str);
}

string array GetId() {
    string tmp;
    //if(!inherits(LIB_GERM,this_object())) tmp = GetKeyName();
    tmp = GetKeyName();

    if( tmp ) {
	if(!OBJECT_MATCHING) return distinct_array(({ Id..., tmp }));
	else return Id + ({ file_name(this_object()) }) + atomize_string(tmp) - ExcludedIds;
    }
    else return Id;
}

string array GetCanonicalId() {
    string tmp;
    tmp = GetKeyName();

    return copy(CanonicalId);
}

varargs string array SetId(mixed val...) {
    if( stringp(val) ) {
	val = ({ val });
    }
    else if( !arrayp(val) ) {
	error("Bad argument 1 to SetId().\n");
    }
    Id = ({});
    foreach(mixed id in val) {
	if( stringp(id) ) {
	    Id = ({ Id..., id });
	}
	else if( arrayp(id) ) {
	    Id = ({ Id..., id... });
	}
    }

    if(COMPAT_MODE) parse_init();
    parse_refresh();

    CanonicalId = Id;

    if(OBJECT_MATCHING){
	Id = atomize_array(Id);
    }
    return Id;
}

string GetKeyName() {
    return KeyName;
}

string SetKeyName(string nom) {
    if( !stringp(nom) ) {
	error("Bad argument 1 to SetKeyName().\n");
    }
    KeyName = lower_case(nom);
    if( !CapName ) {
	CapName = capitalize(nom);
    }
    return KeyName;
}

string GetName() {
    return GetCapName();
}

int id(string str) {
    if( !stringp(str) ) {
	return 0;
    }
    return (member_array(lower_case(str), this_object()->GetId()) != -1);
}

string array parse_command_id_list() {
    string array ids = (this_object()->GetId() || ({}));

    return filter(ids, (: stringp($1) && ($1 != "") :));
}

string array parse_command_plural_id_list() {
    string array ids = (this_object()->GetId() || ({}));

    ids = filter(ids, (: stringp($1) && ($1 != "") :));
    return map(ids, (: pluralize :));
}

string array parse_command_adjectiv_id_list() {
    return filter(this_object()->GetAdjectives(), (: $1 && ($1 != "") :));
}

varargs void eventAnnounceCanonicalId(object env){
    object *inv;
    if(!OBJECT_MATCHING) return;
    if(!env) env = environment();
    if(!env) return;
    if(environment(env)) env = environment(env);
    inv = deep_inventory(env) - ({ this_object() });
    if(sizeof(inv) > 25) return;
    //tc("I am "+identify(this_object())+" and I'm trying to announce to: "+identify(inv),"green");
    inv->ReceiveCanonicalId(CanonicalId);
    inv = all_inventory(this_object());
    if(inv && sizeof(inv)) inv->eventAnnounceCanonicalId(env);
}


varargs void ReceiveCanonicalId(mixed foo, int leaving){
    if(!OBJECT_MATCHING) return;
    if(!foo || !sizeof(foo)) return;
    if(!leaving){
	foreach(mixed element in foo){
	    if(member_array(element, this_object()->GetId()) != -1){
		if(member_array(element, CanonicalId) == -1){
		    ExcludedIds += ({ element });
		    //tc("I am: "+identify(this_object())+", I am excluding: "+element,"red");
		    //tc("Ids "+identify(this_object()->GetId()),"red");
		    parse_init();
		    parse_refresh();
		}
	    }
	}
    }
    else {
	foreach(mixed element in foo){
	    ExcludedIds -= ({ element });
	    //tc("I am: "+identify(this_object())+", I am unexcluding: "+element,"blue");
	    parse_init();
	    parse_refresh();
	}
	//if(environment()) eventAnnounceCanonicalId(environment());
    }
    //tc("I am: "+identify(this_object())+", ExcludedIds: "+identify(ExcludedIds));
    if(previous_object() != this_object()){
	//tc("I am: "+identify(this_object())+", and I ant to send a ReceiveCanonicalId to "+identify(previous_object()),"white");
	if(member_array(previous_object(), NotifiedObjects) == -1){
	    //previous_object()->ReceiveCanonicalId(CanonicalId);
	    //tc("I am: "+identify(this_object())+", I am sending a ReceiveCanonicalId to "+identify(previous_object()),"cyan");
	    //tc("previous: "+identify(previous_object())+" NotifiedObjects: "+identify(NotifiedObjects),"white");
	    NotifiedObjects += ({ previous_object() });
	    //tc("previous: "+identify(previous_object())+" NotifiedObjects: "+identify(NotifiedObjects));
	    previous_object()->ReceiveCanonicalId(CanonicalId);
	}
    }
}
