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

string space_trim(string str) {
    string whitespace;

    whitespace = "  " ;
    while(grepp(str, whitespace)) {
	str = replace_string(str,"  "," ");
    }
    return str;
}

string newline_trim(string str) {
    string *whitespace;
    int j, i = 0;

    j = strlen(str) - 1;
    whitespace = ({ '\n' });
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
    if(sizeof(str) < 7900) return sprintf("%-=" + x + "s\n", str);
    else {
	string tmpfile = generate_tmp();
	write_file(tmpfile,str);
	str = read_bytes(tmpfile,0,7900);
	rm(tmpfile);
	str += "\n*** TRUNCATED ***\n";
	return sprintf("%-=" + x + "s\n", str);
    } 
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
		mixed cle;

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

//this sefun courtesy of Duuk@Haven
string reverse_string(string str) {
    int i;
    string newstring = "";

    if (!str) error("The reverse_string sefun needs input.");

    for (i = strlen(str); i >= 0; i--) {
	newstring += str[i..i];
    }
    return newstring;
}


int starts_with(string primary, string sub){
    string rev, junk; 
    //primary = replace_string(primary,"\t"," ");
    if(!primary) return 0;
    rev = reverse_string(primary);
    rev = trim(rev);
    if(!rev) return 0;
    primary = reverse_string(rev);
    if(!sscanf(primary,sub+" %s",junk) > 0) 
	sscanf(primary,sub+"%s",junk);
    if(junk) return 1;

    else return 0;
}

int starts_with_arr(string primary, string *sub){
    string rev, junk;
    if(!primary ||!stringp(primary)) return 0;
    if(!rev = reverse_string(primary)) return 0;
    rev = trim(rev);
    if(!rev) return 0;
    primary = reverse_string(rev);
    foreach(string element in sub){
	if(sscanf(primary,element+" %s",junk) > 0) return 1;
    }
    return 0;
}

// returns the last [i] characters of a string.
varargs string last(string str, int i, int significant){
    string ret, tmp;
    ret = str[(strlen(str) - i )..(strlen(str) -1)];
    if(significant) {
	tmp = trim(str);
	ret = tmp[(strlen(tmp) - i )..(strlen(tmp) -1)];
    }
    return ret;
}

// returns the first [i] characters of a string.
string first(string str, int i){
    return str[0..(i-1)];
}

// return a string with the last [i] characters removed.
string truncate(string str, int i){
    return str[0..(sizeof(str) - i - 1 )];
}

// remove_matching_line will search the string provided,
// and delete lines that match the search substring. If no number is
// given as the third argument, it will return a string with
// the first line that matches the substring removed. If a
// number is given as a third argument, it returns a string
// with all matching lines removed.
varargs mixed remove_matching_line(string target, string substring, int i, string exclude){
    int omit, done, tail_search;
    string line, filename, new_file;

    if(!target || !file_exists(target)) true();
    else target = read_file(target);
    if(!target) return 0;
    if(strsrch(target,substring) == -1) return target;
    if(strsrch(target,"\n") == -1) return 0;
    filename = "/tmp/"+random(time())+time()+".tmp";
    new_file = "";
    if(!exclude) exclude = filename;

    write_file(filename,target);

    for(i=1; !done; i++){
	line = read_file(filename, i, 1);
	if(!line) break;
	if(strsrch(line,substring) != -1 && strsrch(line,exclude) == -1) omit =1;

	if(omit && last(line[0..strlen(line)-2],1,1) != ";") {
	    tail_search = 1;
	}
	else tail_search = 0;

	if(!omit && !tail_search) {
	    new_file += line;
	}


	if(!tail_search) omit = 0;
	if(!line) done = 100;
	if(i == 999) done = 100;
    }

    rm(filename);
    return new_file;
}

varargs mixed read_matching_line(string target, string substring, int i, string exclude){
    string *ret_array;
    int omit, done, tail_search;
    string wtf, line, filename, new_file;

    if(i && i > 0) wtf = "wtf";
    if(!target || !file_exists(target)) true();
    else target = read_file(target);
    if(!target) return 0;
    if(strsrch(target,substring) == -1) return target;
    if(strsrch(target,"\n") == -1) return 0;
    filename = "/tmp/"+random(time())+time()+".tmp";
    new_file = "";
    if(!exclude) exclude = filename;

    write_file(filename,target);

    for(i=1; !done; i++){
	line = read_file(filename, i, 1);
	if(!line) break;
	if(strsrch(line,substring) != -1 && strsrch(line,exclude) == -1) omit =1;
	if(omit && last(line[0..strlen(line)-2],1,1) != ";") {
	    tail_search = 1;
	}
	else tail_search = 0;

	if(omit || tail_search) {
	    new_file += line;
	}


	if(!tail_search) omit = 0;
	if(!line) done = 100;
	if(i == 999) done = 100;
    }

    rm(filename);
    ret_array = explode(new_file,"\n");
    //return ret_array[0];
    if(wtf) return new_file;
    else return ret_array[0];
}


varargs mixed replace_matching_line(string target, string substring, string replace, int i, string exclude){
    int omit, done, tail_search, tag_it;
    string line, filename, new_file;

    tag_it = 0;
    omit = 0;

    if(!target || !file_exists(target)) true();
    else target = read_file(target);
    if(!target) return 0;
    if(strsrch(target,substring) == -1) return target;
    if(strsrch(target,"\n") == -1) return 0;
    filename = "/tmp/"+random(time())+time()+".tmp";
    new_file = "";

    write_file(filename,target);

    for(i=1; !done; i++){
	line = read_file(filename, i, 1);
	if(!line) break;
	//write("line: "+line);
	if(strsrch(line,substring) != -1 ) omit =1;
	//tc("omit: "+omit);

	if(omit && last(line[0..strlen(line)-2],1,1) != ";") {
	    tail_search = 1;
	    //write("tail_search: "+tail_search);
	}
	else {
	    tail_search = 0;
	    if(omit) tag_it = 1;
	    //tc("tail_search: "+tail_search+", tag_it: "+tag_it+", omit: "+omit);
	}

	if(!omit && !tail_search) {
	    new_file += line;
	    //write("adding the line.");
	}

	if(tag_it == 1){
	    //tc("adding the replace");
	    new_file += replace+"\n";
	    tag_it = 0;
	}

	if(!tail_search) omit = 0;
	if(!line) done = 100;
	if(i == 999) done = 100;
    }

    rm(filename);
    return new_file;
}

int grepp(string primary, string sub){
    if(!primary || ! sub) return 0;
    if(strsrch(primary,sub) != -1) return 1;
    else return 0;
}

int memberp(mixed *primary, mixed sub){
    if(member_array(sub,primary) != -1) return 1;
    else return 0;
}

int reverse_memberp(string primary, string *sub){
    foreach(string element in sub){
	if(strsrch(primary,element) != -1) return 1;
    }
    return 0;
}

string replace_line(string file, string *params, string repl){
    string *file_arr;
    int alarm;

    file_arr = explode(file, "\n");

    foreach(string line in file_arr){

	alarm = 0;

	foreach(string element in params){
	    if(grepp(line,element)) alarm++;
	}

	if(alarm == sizeof(params)){
	    file_arr[member_array(line,file_arr)] = repl;
	}
    }

    return implode(file_arr,"\n");
}

string last_string_element(string str, string delimiter){
    string rev, revd, revret, junk;
    if(!str || !delimiter) return "";
    if(!grepp(str,delimiter)) return "";
    rev = reverse_string(str);
    revd = reverse_string(delimiter);
    sscanf(rev,"%s"+revd+"%s",revret,junk);
    if(!revret || revret == "") return "";
    return reverse_string(revret);
}

varargs string first_string_element(string str, string delimiter, int stripfirst){
    string ret, junk;
    if(!str || !delimiter) return "";
    if(!grepp(str,delimiter)) return "";
    if(stripfirst) str = str[1..sizeof(str)-1];
    sscanf(str,"%s"+delimiter+"%s",ret,junk);
    if(!ret || ret == "") return "";
    return ret;
}

string path_prefix(string str){
    int i = sizeof(str) - sizeof(last_string_element(str,"/"));
    return str[0..i-2];
}

mixed homedir(object ob){
    if(creatorp(ob)) return "/realms/"+ob->GetKeyName();
    else return 0;
}

varargs string generate_tmp(mixed arg){
    string ret;
    string randy = replace_string(replace_string(crypt(""+random(88)+11,""+random(88)+11),"/","XXX"),".","YYY");

    if(!arg) return "/open/"+time()+".c";


    if(objectp(arg) && this_player() && creatorp(this_player()))
	ret = homedir(this_player())+"/tmp/"+last_string_element(base_name(arg),"/")+randy+time()+".c";

    else if(objectp(arg) && this_player())
	ret = "/tmp/"+last_string_element(base_name(arg),"/")+randy+time()+".c";

    else if(stringp(arg) && this_player() && creatorp(this_player())) {
	if(file_exists(arg)) ret = homedir(this_player())+"/tmp/"+last_string_element(arg,"/")+randy+time()+".c";
	else ret = homedir(this_player())+"/tmp/"+randy+time()+".c";
	//ret = homedir(this_player())+"/tmp/"+last_string_element(arg,"/")+randy+time()+".c";
    }

    else if(stringp(arg) && this_player()) {
	if(objectp(load_object(arg))) ret = "/tmp/"+last_string_element(arg,"/")+randy+time()+".c";
	else ret = "/open/"+last_string_element(arg,"/")+randy+time()+".tmp";
    }

    else if(creatorp(this_player())) ret = homedir(this_player())+"/tmp/"+randy+time()+".tmp";

    else ret = "/open/"+randy+time()+".c";
    return ret;
}

int alphap(mixed arg){
    string *alphabet = ({ "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z" });
    alphabet += ({ "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z" });
    if(!stringp(arg)) return 0;
    foreach(string element in alphabet){
	if(grepp(arg,element)) return 1;
    }
    return 0;
}

int numericp(mixed arg){
    string *alphabet = ({"1", "2", "3", "4", "5", "6", "7", "8", "9", "0"});
    if(intp(arg)) arg = itoa(arg);
    if(!stringp(arg)) return 0;
    foreach(string element in alphabet){
	if(grepp(arg,element)) return 1;
    }
    return 0;
}

int basic_punctuationp(mixed arg){
    string *alphabet = ({",", ".", "-", "_", "+", "#", ";", "^", "&", "(", ")" });
    alphabet += ({ "@", "!", "$", "%", "=", "{", "}", "[", "]", ":", "<", ">" });
    if(!stringp(arg)) return 0;
    foreach(string element in alphabet){
	if(grepp(arg,element)) return 1;
    }
    return 0;
}




