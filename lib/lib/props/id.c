/*    /lib/props/id.c
 *    From the Dead Souls V Object Library
 *    Attributes which identify objects
 *    Created by Descartes of Borg 961222
 *    Version: @(#) id.c 1.2@(#)
 *    Last modified: 96/12/22
 */

private static string array Adjectives   = ({});
private string              CapName      = 0;
private static string array Id           = ({});
private static string       KeyName      = 0;

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
    string tmp = GetKeyName();

    if( tmp ) {
	return distinct_array(({ Id..., tmp }));
    }
    else return Id;
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
    parse_refresh();
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
    return (member_array(lower_case(str), GetId()) != -1);
}

string array parse_command_id_list() {
    string array ids = (GetId() || ({}));

    return filter(ids, (: stringp($1) && ($1 != "") :));
}

string array parse_command_plural_id_list() {
    string array ids = (GetId() || ({}));

    ids = filter(ids, (: stringp($1) && ($1 != "") :));
    return map(ids, (: pluralize :));
}

string array parse_command_adjectiv_id_list() {
    return filter(GetAdjectives(), (: $1 && ($1 != "") :));
}
