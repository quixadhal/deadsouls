/*    /adm/sefun/strings.c
 *    from Foundation II
 *    sefuns for string manipulation
 *    created by Descartes of Borg 940506
 */

#include <function.h>
#include <daemons.h>

varargs string center(string str, int x) {
    int y;

    if(!x) x= 80;
    x--;
    if((y = strlen(strip_colours(str))) >= x) return str;
    x = x + strlen(str) - y;
    return sprintf(sprintf("%%|%ds", x), str);
}

varargs string arrange_string(string str, int x) {
    int y;

    if(!x) x = 80;
    x += strlen(str) - strlen(strip_colours(str));
    return sprintf(sprintf("%%:-%ds", x), str);
}

string evaluate_description(mixed val) {
    if( stringp(val) ) {
	return val;
    }
    if( functionp(val) ) {
	if( functionp(val) & FP_OWNER_DESTED ) {
	    return 0;
	}
	else {
	    return evaluate(val);
	}
    }
    if( arrayp(val) ) {
	return evaluate_description(val[query_night()]);
    }
    return val;
}

string to_html(string str) {
    return TERMINAL_D->GetHTML(str);
}

string trim(string str) {
    string *whitespace;
    int j, i = 0;
 
    j = strlen(str) - 1;
    whitespace = ({ ' ', '\t' });
    while(member_array(str[i], whitespace) != -1) i++;
    while(i < j  && member_array(str[j], whitespace) != -1) j--;
    return str[i..j];
}

varargs string wrap(string str, int x) {
    if( !x ) {
        if( !this_player() ) x = 79;
        else {
            int *tmp;

            tmp = (int *)this_player()->GetScreen();
            if( tmp ) x = (tmp[0] || 79);
            else x = 79;
        }
    }
    return sprintf("%-=" + x + "s\n", str);
}

varargs mixed convert_string(string str, int flag) {
    mixed *ret = ({ 0, "" });

    if( (str = trim(str)) == "" ) return 0;
    if( str[0] == '(' ) {
	switch(str[1]) {

	    case '{':

	    ret[0] = ({});
	    str = str[2..];
	    while(str[0] != '}') {
		mixed *tmp;

		tmp = convert_string(str, 1);
		ret[0] += ({ tmp[0] });
		str = tmp[1];
		while(str[0] == ' ' || str[0] == '\t') str = str[1..];
		if( str[0] != ',' && str[0] != '}' )
		  error("Improperly formatted array: " + str + "\n");
		else if( str[0] == ',') {
		    str = str[1..];
		    while(str[0] == ' ' || str[0] == '\t') str = str[1..];
		}
	    }
	    if( str[1] != ')' ) {
		str = str[2..];
		while(str[0] == ' ' || str[0] == '\t') str = str[1..];
		if( str[0] != ')' ) error("Illegal array terminator.\n");
		else ret[1] = str[1..];
	    }
	    else ret[1] = str[2..];
	    if( !flag ) return ret[0];
	    while(ret[1][0] == ' ' || ret[1][0] == '\t') ret[1] = ret[1][1..];
	    return ret;

	    case '[':

	    ret[0] = ([]);
	    str = str[2..];
	    while(str[0] != ']') {
		mixed *tmp;
		mixed cle, val;

		tmp = convert_string(str, 1);
		str = tmp[1];
		while(str[0] == ' ' || str[0] == '\t') str = str[1..];
		if( str[0] != ':' ) 
		  error("Illegally formatting mapping: " + str + "\n");
		cle = tmp[0];
		tmp = convert_string(str[1..], 1);
		ret[0][cle] = tmp[0];
		str = tmp[1];
		while(str[0] == ' ' || str[0] == '\t') str = str[1..];
		if( str[0] != ',' && str[0] != ']' ) 
		  error("Illegally formatted mapping: " + str + "n");
		else if( str[0] != ']' ) {
		    str = str[1..];
		    while(str[0] == ' ' || str[0] == '\t') str = str[1..];
		}
	    }
	    if( str[1] != ')' ) {
		str = str[2..];
		while(str[0] == ' ' || str[0] == '\t') str = str[1..];
		if( str[0] != ')' ) error("Illegal array terminator.\n");
		else ret[1] = str[1..];
	    }
	    else ret[1] = str[2..];
	    if( !flag ) return ret[0];
	    while(ret[1][0] == ' ' || ret[1][0] == '\t') ret[1] = ret[1][1..];
	    return ret;
        }
    }
    else if( str[0] == '"' ) {
	string tmp;

	tmp = "";
	while( str[1] != '"' || (str[1] == '"' && str[0] == '\\') ) {
	    if( str[1] == '"' ) tmp = tmp[0..<2] + "\"";
	    else tmp += str[1..1];
	    str = str[1..];
	}
	if( !flag ) return tmp;
	if( strlen(str) > 2 ) str = trim(str[2..]);
	return ({ tmp, str });
    }
    else if( str[0] >= '0' && str[0] <= '9' || str[0] == '-' ) {
	string tmp;
	int y;
	
	if( strlen(str) > 1 && str[0] == '-' ) {
	    tmp = str[0..0];
	    str = str[1..];
	}
	else {
	    tmp = "";
	}
	if( strlen(str) > 1 && str[0..1] == "0x" ) {
	    tmp = "0x";
	    str = str[2..];
	    while(str != "" && (str[0] >= '0' && str[0] <= '9')) {
		tmp += str[0..0];
		if( strlen(str) > 1 ) str = str[1..];
		else str = "";
	    }
	    sscanf(tmp, "%x", y);
	}
	else {
	    while(str != "" && (str[0] >= '0' && str[0] <= '9')) {
		tmp += str[0..0];
		if( strlen(str) > 1 ) str = str[1..];
		else str = "";
	    }
	    sscanf(tmp, "%d", y);
	}
	if( !flag ) return y;
	if( str != "" ) str = trim(str);
	return ({ y, str });
    }
    else {
	string tmp;

	tmp = "";
	while(strlen(str) && ((str[0] >= 'a' && str[0] <= 'z') ||
			      (str[0] >= 'A' && str[0] <= 'Z') ||
			      (str[0] >= '0' && str[0] <= '9') ||
			      (str[0] == '_'))) {
	    tmp += str[0..0];
	    if( strlen(str) > 1 ) str = str[1..];
	    else str = "";
	}
	if( !flag ) return to_object(tmp);
	else return ({ to_object(tmp), str });
    }
    error("Gobbledygook in string.\n");
}
