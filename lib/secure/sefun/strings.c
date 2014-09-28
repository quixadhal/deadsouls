/*    /adm/sefun/strings.c
 *    from Foundation II
 *    sefuns for string manipulation
 *    created by Descartes of Borg 940506
 */

#include <function.h>
#include <daemons.h>
#include <message_class.h>
#include <runtime_config.h>

string global_temp_file = "";

mapping Morse = ([ "a" : ".-", "b" : "-...", "c" : "-.-.",
        "d" : "-..", "e" : ".", "f" : "..-.", "g" : "--.", "h" : "....", "i" : "..",
        "j" : ".---", "k" : "-.-", "l" : ".-..", "m" : "--", "n" : "-.", "o" : "---",
        "p" : ".--.", "q" : "--.-", "r" : ".-.", "s" : "...", "t" : "-", "u" : "..-",
        "v" : "...-", "w" : ".--", "x" : "-..-", "y" : "-.--", "z" : "--..",
        "1" : ".----", "2" : "..---", "3" : "...--", "4" : "....-", "5" : ".....",
        "6" : " -....", "7" : "--...", "8" : "---..", "9" : "----.","0" : " -----",
        "." : ".-.-.-", "," : "--..--", "?" : "..--..", "'" : ".----.", "!" : "-.-.--",
        "/" : "-..-.", "(" : "-.--.", ")" : "-.--.-", "&" : ".-...", ":" : "---...",
        ";" : "-.-.-.", "=" : "-...-", "+" : ".-.-.", "-" : "-....-", "_" : "..--.-",
        "\"" : ".-..-.", "$" : "...-..-", "@" : ".--.-." ]);

varargs string center(string str, int x) {
    int y;

    if(!x) x= 80;
    x--;
    if((y = strlen(strip_colours(str))) >= x) return str;
    x = x + strlen(str) - y;
    return sprintf(sprintf("%%|%ds", x), str);
}

varargs string arrange_string(string str, int x) {
    string orig = str;
    string bare = strip_colors(str);
    if(!x) x = 80;
    x += (strlen(orig) - strlen(bare));
    return sprintf(sprintf("%%:-%ds", x), str);
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

    if(!str || !sizeof(str) || str == "") return 0;

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
    if(!str || !sizeof(str) || !stringp(str)) return "";
    return str[<i..];
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


//If you try to use replace_matching_line and it doesn't work,
//it may be that the file in question is not a .c file. This sefun
//keys on .c file syntax to know where lines begin and end. If the
//string you're editing doesn't have, for example, semicolons, 
//your line replacememnt probably won't work well. Instead use the 
//sefun replace_line.
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
        if(strsrch(line,substring) != -1 ) omit =1;

        if(omit && last(line[0..strlen(line)-2],1,1) != ";") {
            tail_search = 1;
        }
        else {
            tail_search = 0;
            if(omit) tag_it = 1;
        }

        if(!omit && !tail_search) {
            new_file += line;
        }

        if(tag_it == 1){
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

//This sefun is for non-code text. To replace lines in an LPC-code
//formatted string, use replace_matching_line instead.
string replace_line(string file, string *params, string repl){
    string *file_arr;
    int alarm;

    if(!file || !stringp(file)) return "";
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

string append_line(string file, mixed params, string repl){
    string *file_arr;
    int alarm;

    if(!file || !stringp(file)) return "";
    file_arr = explode(file, "\n");

    if(params){
        if(stringp(params)) params = ({ params });
    }

    foreach(string line in file_arr){
        alarm = 0;

        foreach(string element in params){
            if(grepp(line,element)) alarm++;
        }

        if(alarm == sizeof(params)){
            file_arr[member_array(line,file_arr)] += ("\n"+repl);
        }

    }
    return implode(file_arr,"\n");
}

string last_string_element(string str, string delimiter){
    mixed ret;
    if(!str || !delimiter || !grepp(str,delimiter)) return "";
    ret = explode(str,delimiter);
    if(sizeof(ret)) return ret[<1..][0];
    return "";
}

varargs string first_string_element(string str, string delimiter, int stripfirst){    
    if(!str || !delimiter || !grepp(str,delimiter)) return "";
    return explode(str,delimiter)[0];
}

string path_prefix(string str){
    int i = sizeof(str) - sizeof(last_string_element(str,"/"));
    return str[0..i-2];
}

varargs mixed homedir(mixed ob, int cre){
    string name = "";
    string initial = "";
    if(!ob) ob = this_player();
    if(!ob) return 0;
    if(objectp(ob)) name = ob->GetKeyName();
    else if(stringp(ob)) name = ob;
    else return 0;
    if(!sizeof(name)) return 0;
    initial = name[0..0];
    if(!user_exists(name)) return 0;
    if(cre || directory_exists("/realms/"+name)) return "/realms/"+name;
    else return DIR_ESTATES + "/"+initial+"/"+name; 
}

varargs mixed random_numbers(int n, int integer){
    string ret = "";
    int i;
    if(integer && n > 9) n = 9;
    for(i=n;i>0;i--){
        //int tmp = 1;
        int tmp = random(10);
        if(!sizeof(ret)) tmp = random(9)+1;
        ret += itoa(tmp);
    }
    if(!integer) return ret;
    else return(atoi(ret));
}

varargs mixed alpha_crypt(mixed arg1, mixed arg2){
    string ret;
    if(!intp(arg1) && !arg2) return 0;
    if(intp(arg1)) {
        if(arg1 > 64) arg1 = 64;
        //ret = crypt(""+random(arg1+2)+arg1,""+random(arg1+2)+arg1);
        //ret += crypt(""+random(arg1+2)+arg1,""+random(arg1+2)+arg1);
        ret = crypt(""+random(arg1+2)+arg1,""+random(999999)*91);
        ret += crypt(""+random(arg1+2)+arg1,""+random(999999)*19);
        ret = replace_string(ret,"`","");
        ret = replace_string(ret,"!","");
        ret = replace_string(ret,"/","");
        ret = replace_string(ret,".","");
        return ret[0..(arg1 - 1)];
    }

    ret = crypt(arg1, arg2);
    ret = replace_string(ret,"`","");
    ret = replace_string(ret,"!","");
    ret = replace_string(ret,"/","");
    ret = replace_string(ret,".","");

    return ret;
}


varargs string generate_tmp(mixed arg){
    string ret;
    //string randy = replace_string(replace_string(crypt(""+random(88)+11,""+random(88)+11),"/","XXX"),".","YYY");
    string randy = alpha_crypt(8);

    if(!arg) return "/open/"+time()+"_"+randy+".c";

    if(objectp(arg) && this_player() && builderp(this_player()))
        ret = homedir(this_player())+"/tmp/"+last_string_element(base_name(arg),"/")+randy+time()+".c";

    else if(objectp(arg) && this_player())
        ret = "/tmp/"+last_string_element(base_name(arg),"/")+randy+time()+".c";

    else if(stringp(arg) && this_player() && builderp(this_player())) {
        if(file_exists(arg)) ret = homedir(this_player())+"/tmp/"+last_string_element(arg,"/")+randy+time()+".c";
        else ret = homedir(this_player())+"/tmp/"+randy+time()+".c";
        //ret = homedir(this_player())+"/tmp/"+last_string_element(arg,"/")+randy+time()+".c";
    }

    else if(stringp(arg) && file_exists(arg) && this_player()) {
        if(objectp(load_object(arg))) ret = "/tmp/"+last_string_element(arg,"/")+randy+time()+".c";
        else ret = "/open/"+last_string_element(arg,"/")+randy+time()+".tmp";
    }

    else if(builderp(this_player())) ret = homedir(this_player())+"/tmp/"+randy+time()+".tmp";

    else ret = "/open/"+randy+time()+".c";
    return ret;
}

int alphap(mixed arg){
    if(!stringp(arg)) return 0;
    foreach(int element in arg){
        if(!((element >= 65 && element <= 90) ||
                    (element >= 97 && element <= 122))) return 0;
    }
    return 1;
}

string alpha_strip(mixed arg){
    string raw, ret;
    string *blown;
    if(!arg || pointerp(arg)) return "";
    if(!sscanf(arg,"%s",raw)) return "";
    if(!sizeof(raw)) return "";
    blown = explode(raw,"");
    ret = "";
    foreach(string element in blown){
        if(alphap(element)) ret += element;
    }
    return ret;
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

int check_string_length(string str){
    if(sizeof(str) > __LARGEST_PRINTABLE_STRING__) return 0;
    return 1;
}

varargs string *chunk_string(string str, int width){
    string *ret =({});
    string tmp1 = "";
    string tmpfile = generate_tmp();
    if(!width) width = __LARGEST_PRINTABLE_STRING__ / 2;
    while(sizeof(str)){
        write_file(tmpfile,str,1);
        if(!(tmp1 = read_bytes(tmpfile, 0, width)))
            tmp1 = read_file(tmpfile);
        ret += ({ tmp1 });
        str = replace_string(str,tmp1,"");
    }
    rm(tmpfile);
    return ret;
}

varargs mixed print_long_string(object who, string str, int catted){
    string ret = "";
    string *lines;
    string *tmp;
    if(!str) return 0;
    global_temp_file = generate_tmp();
    lines = explode(str,"\n");
    foreach(string line in lines){
        if(sizeof(line) > __LARGEST_PRINTABLE_STRING__ / 2) 
            line = implode(chunk_string(line,who->GetScreen()[0]),"\n");
        ret += line+"\n";
    }
    write_file(global_temp_file,ret,1);
    tmp = explode(ret,"\n");
    foreach(string thing in tmp){
    }
    if(!catted){
        who->eventPage(explode(read_file(global_temp_file),"\n"),MSG_SYSTEM);
        return unguarded( (: rm(global_temp_file) :) );
    }
    else {
        foreach(string thing in tmp){
            message("system", thing, who);
        }
    }
    unguarded( (: rm(global_temp_file) :) );
    return 1;
}

string convert_newline(string str){
    string ret = "";
    if(!str) return ret;
    ret = replace_string(str,CARRIAGE_RETURN,"\n");
    return ret;
}

int clean_newline_file(string str){
    string ret = "";
    if(!file_exists(str)) return 0;
    else ret = convert_newline(read_file(str));
    if(ret != "") return write_file(str,ret,1);
    else return 0;
}

string morse(string msg) {
    mapping morse = Morse;
    string tmp;
    int x, i;
    msg = lower_case(msg);
    for(tmp = "", x = strlen(msg), i=0; i< x; i++) {
        if(morse[msg[i..i]]) tmp += morse[msg[i..i]]+" ";
        else tmp += msg[i..i]+ " ";
    }
    return tmp;
}

string unmorse(string msg) {
    mapping morse = ([]);
    string tmp = "";
    string *phrase = ({});
    int x, i;
    foreach(mixed key, mixed val in Morse){
        morse[val] = key;
    }

    phrase = explode(msg,"  ");
    foreach(string element in phrase){
        string *word = explode(element," ");
        foreach(string letter in word){
            if(member_array(letter, keys(morse)) != -1){
                tmp += morse[letter];
            }
        }
        tmp += " ";
    } 
    return tmp;
}

string unpinkfish(string str){
    if(!str) error("String required.");
    str = replace_string(str,"\%\%\^\^","0^^0");
    str = replace_string(str,"\%\^","\%\%\^\^");
    str = replace_string(str,"0^^0","\%\%\^\^");
    return str;
}

string repinkfish(string str){
    if(!str) error("String required.");
    str = replace_string(str,"\%\%\^\^","\%\^");
    return str;
}

string web_translate(string str){
    if(!str) error("String required");
    str = replace_string(str,"%2F","/");
    str = replace_string(str,"%2B","+");
    return str;
}

string dbz_colors(string str, int annoying){
    string ret = "";
    string *colors = ({ "RED", "BLUE", "CYAN", "MAGENTA", "ORANGE",
            "YELLOW", "GREEN", "WHITE%^%^B_BLACK", "BLACK%^%^B_WHITE" });
    string *b_colors = ({ "B_RED", "B_BLUE", "B_CYAN", "B_MAGENTA", "B_ORANGE",
            "B_YELLOW", "B_GREEN" });

    foreach(mixed element in str){
        int close;
        if(random(2)) ret += "%^BOLD%^";
        if(annoying && !random(3) && element != 32){
            ret += "%^" + b_colors[random(sizeof(b_colors)-1)] + "%^";
            close = 1;
        }
        if(annoying > 1 && !random(5) && element != 32){
            ret += "%^FLASH%^";
            close = 1;
        }
        ret += "%^" + colors[random(sizeof(colors)-1)] + "%^";
        ret += convert_ascii(element);
        if(close) ret += "%^RESET%^";
    }
    return ret + "%^RESET%^";
}

int query_common_ascii(string str){
    mixed *check_arr = ({});
    mixed *ret_arr = ({});
    int i;
    int lst = sizeof(str) - 1;
    if(sizeof(str) == 1){ 
        check_arr = ({ str[0] });
    }
    else {
        for(i = 0; i < lst; i++){
            check_arr += ({ str[i] });
        }
    }
    foreach(mixed element in check_arr){
        if(intp(element)){
            if((element >31 && element < 127)
                    || element == 10 || element == 9 ||
                    (element >160 && element < 256)){
                ret_arr += ({ element });
            }
        }
    }
    if(sizeof(ret_arr) == sizeof(check_arr)) return 1;
    return 0;
}

string cleaned_name(string str){
    if(last(str, 2) == ".o") str = truncate(str, 2);
    str = replace_string(str, "."+__PORT__, "");
    return str;
}

string cleaned_end(string str){
    str = last_string_element(str, "/");
    str = cleaned_name(str);
    return str;
}

string *read_big_file(string str) {
    string *arr, *ret = ({});
    mixed tmp;
    string stub;
    int bigth, ender, bmax = (__LARGEST_PRINTABLE_STRING__ - 1024);
    int chunks, rem, currchunk, smax = (get_config(__MAX_STRING_LENGTH__) - 10);

    if( !str || !file_exists(str) ) return ret;
    if((bigth = file_size(str)) < smax){
        tmp = read_file(str);
        ret = explode(tmp,"\n");
        return ret;
    }
    chunks = bigth / bmax;
    rem = bigth % bmax;
    if(rem) chunks++;
    while(chunks){
        chunks--;
        if(chunks){
            tmp = read_bytes(str, currchunk, bmax);           
            currchunk += bmax;
        }
        if(rem && !chunks){
            tmp = read_bytes(str, currchunk, rem);
            currchunk += rem;
        }
        if(tmp[<1..] == "\n") ender = 1;
        else ender = 0;
        arr = explode(tmp, "\n");
        if(sizeof(stub)) {
            arr[0] = stub+arr[0];
            stub = "";
        }
        if(ender){
            ret += arr;
        }
        else {
            stub = arr[<1..][0];
            ret += arr[0..<2];
        }

    }
    return ret;
}

