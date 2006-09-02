/*    /lib/user/nmsh.c
 *    from Dead Souls
 *    the new Dead Souls shell
 *    created by Descartes of Borg 940216
 *    Version: @(#) nmsh.c 1.3@(#)
 *    Last Modified: 96/10/15
 */

#include <config.h> 
#include <daemons.h>
#include <lib.h>
#include "include/nmsh.h"

inherit LIB_HISTORY;

#define DIRECTORY_STACK_SIZE     5
#define MAX_CMD_ALIASES          128

private string CurrentWorkingDirectory;
private string PreviousWorkingDirectory;
private mapping Nicknames, Aliases, Xverbs; 
private static int CWDCount, CWDBottom, CWDTop, CmdNumber; 
private static string Prompt, CommandFail; 
private static string *Stack; 

static void create() {
    history::create();
    Nicknames = ([]); 
    Aliases = ([ "l" : "look $*", "bio" : "biography", "i" : "inventory",
      "n" : "go north", "s" : "go south", "e" : "go east", "w" : "go west",
      "ne" : "go northeast", "nw" : "go northwest", "se" : "go southeast",
      "sw" : "go southwest", "d" : "go down", "u" : "go up", "out": "go out",
      "exa" : "look at $*", "p" : "people", "sc" : "status", "inf" : "score",
      "eq" : "inventory", "where" : "people", "prac" : "skills", 
      "sco" : "score", "practice" : "skills", "trophy" : "kills",
      "northwest" : "go northwest", "northeast" : "go northeast", 
      "southwest" : "go southwest", "southeast" : "go southeast",
      "north" : "go north", "south": "go south", "east" : "go east",
      "west" : "go west", "up" : "go up", "down": "go down", 
      "ig" : "intergossip $*", "c" : "cre $*", "lp" : "lpuni $*",
      "inv" : "inventory", "x" : "look at $*", "examine" : "look at $*",
      "ic" : "intercre $*", "aa" : "ascii_art $*", "chat" : "newbie $*"
    ]);
    Xverbs = (["]":"] $*", "'":"say $*",":":"emote $*","\"":"say $*",]);
} 

int Setup() { 
    if(this_player() != this_object()) return 0; 
    reset_prompt(); 
    if(!Nicknames) Nicknames = ([]);
    if(!Aliases) Aliases = ([]);
    if(!Xverbs) Xverbs = ([]);
    add_action("cmd_alias", "alias",1); 
    add_action("cmd_unalias", "unalias",1); 
    add_action("cmd_nickname", "nickname",1); 
    if(creatorp(this_object())) { 
	Stack = allocate(DIRECTORY_STACK_SIZE); 
	CWDBottom = CWDTop = CWDCount = 0; 
	add_action("cmd_cd", "cd"); 
	add_action("cmd_nmsh", "nmsh"); 
	add_action("cmd_pushd", "pushd"); 
	add_action("cmd_popd", "popd"); 
	add_action("cmd_pwd", "cwd");
	add_action("cmd_pwd", "pwd");
	add_action("cmd_work", "work");
    } 
    return 1;
} 

nomask static int cmd_alias(string str) { 
    string *a, *b;
    string key, thing; 
    int i;

    if(this_player() != this_object()) return 0; 
    if(!str) {
	i = sizeof(a = keys(Aliases));
	while(i--)
	    a[i] = sprintf("%s%s", arrange_string(a[i],15),Aliases[a[i]]);
	i = sizeof(b = keys(Xverbs));
	while(i--)
	    b[i] = sprintf("$%s%s", arrange_string(b[i],14), Xverbs[b[i]]);
	this_player()->eventPage(a+b);
	return 1;
    }
    if(sscanf(str, "%s %s", key, thing) != 2) { 
	if(str[0] == '$') { 
	    str = str[1..strlen(str)-1]; 
	    if(Xverbs[str]) { 
		//map_delete(Xverbs, str); 
		//message("system", sprintf("Alias $%s removed.", str), this_player()); 
		write(str+": "+Xverbs[str]);
		return 1;
	    } 
	    else message("system", sprintf("No such alias $%s.", str), this_player()); 
	    return 1; 
	} 
	if(Aliases[str]) { 
	    //map_delete(Aliases, str); 
	    //message("system", sprintf("Alias %s removed.", str), this_player()); 
	    write(str+": "+Aliases[str]);
	    return 1;
	} 
	else message("system", sprintf("No such alias %s.", str), this_player()); 
	return 1; 
    } 
    if(sizeof(Xverbs) + sizeof(Aliases) >= MAX_CMD_ALIASES) { 
	message("system", "You must remove an alias before adding another.", 
	  this_player()); 
	return 1; 
    } 
    if(key == "alias") return notify_fail("That would be a bad idea.\n");
    if(key[0] == '$') { 
	key = key[1..strlen(key)]; 
	if(Xverbs[key])  
	    message("system", sprintf("Alias for $%s altered to (%s).", 
		key, thing), this_player()); 
	else message("system", sprintf("Alias $%s (%s) added.", key, thing), 
	      this_player());      
	Xverbs[key] = thing; 
    } 
    else { 
	if(Aliases[key]) 
	    message("system", sprintf("Alias for %s altered to (%s).", key, thing), 
	      this_player()); 
	else message("system", sprintf("Alias %s (%s) added.", key, thing),this_player()); 
	Aliases[key] = thing; 
    } 
    return 1; 
} 

nomask static int cmd_unalias(string str) { 
    string *a, *b;
    string key, thing; 
    int i;

    if(this_player() != this_object()) return 0; 
    if(!str) {
	write("Unalias what?");
	return 1;
    }
    if(str[0] == '$') { 
	str = str[1..strlen(str)-1]; 
	if(Xverbs[str]) { 
	    map_delete(Xverbs, str); 
	    message("system", sprintf("Alias $%s removed.", str), this_player()); 
	    return 1;
	} 
	else message("system", sprintf("No such alias $%s.", str), this_player()); 
	return 1; 
    } 
    if(Aliases[str]) { 
	map_delete(Aliases, str); 
	message("system", sprintf("Alias %s removed.", str), this_player()); 
	return 1;
    } 
    else message("system", sprintf("No such alias %s.", str), this_player()); 
    return 1; 
} 

nomask static int cmd_cd(string str) { 
    if(this_player() != this_object()) return 0; 
    set_cwd(str); 
    return 1; 
} 

nomask static int cmd_nickname(string str) { 
    string *cles;
    string key, thing; 
    int i;

    if(this_player() != this_object()) return 0;
    if(!str) {
	i = sizeof(cles = keys(Nicknames));
	while(i--)
	    cles[i] = sprintf("%s%s", arrange_string(cles[i], 15),
	      Nicknames[cles[i]]);
	this_player()->eventPage(cles);
	return 1;
    }
    if(sscanf(str, "%s %s", key, thing) != 2) { 
	if(Nicknames[str]) { 
	    message("system", sprintf("Nickname %s removed.", str), 
	      this_player()); 
	    map_delete(Nicknames, str); 
	} 
	else message("system", sprintf("No such nickname %s.", str), 
	      this_player()); 
    } 
    else { 
	if(Nicknames[key])  
	    message("system", sprintf("Nickname %s altered to (%s).", key, thing), 
	      this_player()); 
	else message("system", sprintf("Nickname %s (%s) added.", key, thing), 
	      this_player()); 
	Nicknames[key] = thing; 
    } 
    return 1; 
} 

nomask static int cmd_nmsh(string str) { 
    string *lines; 
    string tmp;
    int i, maxi; 

    if(!str) return 0; 
    if(this_player() != this_object()) return 0; 
    if((int)this_player()->GetForced()) return 0; 
    if(!(tmp = read_file(absolute_path(query_cwd(), str)))) 
	return notify_fail(sprintf("nmsh: script %s not found.\n")); 
    maxi = sizeof(lines = explode(tmp, "\n")); 
    for(i=0; i < maxi; i++) { 
	if(lines[i][0] == '#') continue; 
	if(!command(lines[i])) { 
	    message("system", sprintf("nmsh: error in executing %s.", str), 
	      this_player()); 
	    return 1; 
	} 
    } 
    return 1; 
} 

nomask static int cmd_pushd(string str) { 
    if(this_player() != this_object()) return 0; 
    if(!set_cwd(str)) return 0; 
    pushd(str); 
    return 1; 
} 

nomask static int cmd_popd(string str) { 
    if(this_player() != this_object()) return 0; 
    set_cwd(popd()); 
    return 1; 
} 

nomask static int cmd_pwd(string str) {
    if(!query_cwd()) message("system", "No current directory.", this_object());
    else message("system", query_cwd()+":", this_object());
    return 1;
}

nomask static int cmd_work(string str) {
    string *tmp;
    object ob;
    string file;
    int flag;

    if(!str || str == "") ob = environment(this_object());
    else if(str == "!") {
	flag = 1;
	ob = environment(this_object());
    }
    else if(str[0] == '!') {
	flag = 1;
	str = str[1..strlen(str)];
    }
    if(!ob && !(ob = present(str, this_object())))
	ob = present(str, environment(this_object()));
    if(!ob) return notify_fail("No target object found.\n");
    tmp = explode(file = base_name(ob), "/");
    set_cwd("/"+implode(tmp[0..sizeof(tmp)-2], "/"));
    if(flag) {
	message("system", file+".c, "+file_size(file+".c")+" bytes:",
	  this_object());
	this_object()->eventEdit(file+".c");
    }
    return 1;
}

nomask string write_prompt() { 
    string tmp, ret; 
    int x, y;

    if( (y = query_ed_mode()) != -1 ) {
	if( !y ) {
	    if( creatorp() ) ret = ":";
	    else 
		ret = "\tQ)uit without saving, save and ex)it, h)elp\nCommand: ";
	}
	else if( y == -2 ) ret = "Help: ";
	else ret = "*\b";
	message("prompt", ret, this_object());
	return ret;
    }
    if((ret = Prompt) == DEFAULT_PROMPT) {
	message("prompt", ret, this_object());
	return ret;
    }
    while((x = strsrch(ret, "$")) != -1) {
	if(x == strlen(ret) -1) break;
	switch(ret[x+1]) {
	case 'D': 
	    if(!creatorp(this_object())) break;
	    if(sscanf(query_cwd(), user_path(GetKeyName())+"%s",
		tmp)) tmp = "~"+tmp;
	    else tmp = query_cwd();
	    ret = replace_string(ret, "$D", tmp); 
	    break;
	case 'V': case 'v':
	    if(GetInvis()) {
		ret = replace_string(ret, "$V", "INVIS"); 
		ret = replace_string(ret, "$v", "invis"); 
	    } 
	    else if(hiddenp(this_object())) { 
		ret = replace_string(ret, "$V", "HID"); 
		ret = replace_string(ret, "$v", "hid"); 
	    } 
	    else { 
		ret = replace_string(ret, "$V", ""); 
		ret = replace_string(ret, "$v", ""); 
	    } 
	    break;
	case 'C':
	    ret = replace_string(ret, "$C", sprintf("%d", CmdNumber+1)); 
	    break;
	case 'H':
	    ret = replace_string(ret, "$H", sprintf("%d", query_max_hp())); 
	    break;
	case 'h':
	    ret = replace_string(ret, "$h", sprintf("%d", query_hp())); 
	    break;
	case 'G':
	    ret = replace_string(ret, "$G", sprintf("%d", query_max_mp())); 
	    break;
	case 'g':
	    ret = replace_string(ret, "$g", sprintf("%d", query_mp())); 
	    break;
	case 'I':
	    ret = replace_string(ret, "$I", sprintf("%d", query_max_sp())); 
	    break;
	case 'i':
	    ret = replace_string(ret, "$i", sprintf("%d", query_sp())); 
	    break;
	default:
	    ret = replace_string(ret, ret[x..x+1], "");
	    break;
	}
    }
    message("prompt", ret, this_object());
    return ret;
} 

string process_input(string str) { 
    string tmp, xtra, request; 

    if(!str || str == "") return ""; 
    else if(GetClient() &&
      member_array(GetClient(), SUPPORTED_CLIENTS) != -1) {
	if(sscanf(str, "<%s>%s", request, xtra)) {
	    process_request(request, xtra ? xtra : "");
	    return "";
	}
	else return str;
    }
    else if((tmp = eventHistory(str)) == "") return ""; 
    if(tmp != str) message("system", tmp, this_object());
    return do_alias(do_nickname(tmp));
} 

nomask static void process_request(string request, string xtra) {
    switch(request) {
    case "ALIAS":
	receive("<ALIAS>[n,go north] [s,go south] [e,go east] [w,go west] "
	  "[nw,go northwest] [ne,go northeast] [sw,go southwest] "
	  "[se,go southeast] [u,go up] [d,go down] [i,inventory] "
	  "[bio,biography] [exa,look at $*] [$',say $*] "
	  "[$:,emote $*] [l,look]\n");
	break;
    case "NICKNAME": receive("<NICKNAME>\n"); break;
    case "USERS":
	receive("<USERS>"+implode(map_array(filter(users(), 
		"request_vis", this_object()), "user_names", this_object()), 
	    ", ")+"\n");
	break;
    case "ROOM":
	receive("<ROOM>"+
	  (string)environment(this_object())->GetShort()+"\n");
	break;
    case "PRESENT":
	receive("<PRESENT>"+
	  implode(map_array(filter(all_inventory(environment(this_object())),
		"request_vis", this_object()), "user_names", this_object()),
	    ", ")+"\n");
	break;
    default:
	receive("<error>Request not supported.\n");
	break;
    }
}

static int request_vis(object ob) {
    return (userp(ob) && !((int)ob->GetInvis(this_object())));
}

static string user_names(object ob) {
    return (string)ob->GetName();
}

private static int set_cwd(string str) { 
    int x;
    string tmpstr = str;
    if(str == "~-") str = PreviousWorkingDirectory;
    if(!str || str == "") str = user_path(GetKeyName()); 
    if (str[<1] == '/' && str != "/") str = str[0..<2];
    replace_string(str, "//", "/"); 
    str = absolute_path(query_cwd(), str);
    if(!directory_exists(str) && tmpstr == "here" && environment(this_player())){
	str = path_prefix(base_name(environment(this_player())));
    } 
    if((x=file_size(str)) != -2) { 
	if(x > -1) { 
	    message("system", sprintf("%s: Path is a file.", str), this_player()); 
	    return 0; 
	} 
	else { 
	    message("system", sprintf("%s: No such reference.", str), this_player()); 
	    return 0; 
	}  
    } 

    if(str != query_cwd()) PreviousWorkingDirectory = query_cwd();
    CurrentWorkingDirectory = str; 
    message("system", sprintf("%s:", CurrentWorkingDirectory), this_player()); 
    return 1; 
} 

private static void pushd(string str) { 
    if(CWDCount++ == DIRECTORY_STACK_SIZE) { 
	CWDCount--; 
	CWDBottom = (++CWDBottom) % DIRECTORY_STACK_SIZE; 
    } 
    Stack[CWDTop] = str; 
    CWDTop = (++CWDTop) % DIRECTORY_STACK_SIZE; 
} 

private static string popd() { 
    if(!CWDCount) return 0; 
    CWDCount--; 
    return Stack[--CWDTop]; 
} 

nomask private static string do_nickname(string str) { 
    if(!Nicknames) return str; 
    if(str[0..7] == "nickname") return str; 
    return implode(map_array(explode(str, " "), "replace_nickname", this_object()), " "); 
} 

nomask private static string do_alias(string str) { 
    string *words; 
    string tmp; 
    int x; 

    if(!sizeof(words = explode(str, " "))) return "";
    if((x = strlen(words[0])) && (tmp = Xverbs[words[0][0..0]])) {
	words[0] = words[0][1..x-1];
	return replace_string(tmp, "$*", implode(words, " "));
    }
    if(!(tmp = Aliases[words[0]])) return implode(words, " "); 
    else str = implode(words[1..sizeof(words)-1], " "); 
    return replace_string(tmp, "$*", str); 

} 

nomask static string replace_nickname(string str) { 
    if(str == "") return str; 
    if(str[0] == '\\') return str[1..(strlen(str)-1)]; 
    else if(Nicknames[str]) return Nicknames[str]; 
    else return str; 
} 

void reset_prompt() { 
    Prompt =GetPrompt(); 
    if(!stringp(Prompt)) Prompt = "Prompt screwey> ";
    Prompt =replace_string(Prompt, "$M", mud_name()); 
    Prompt =replace_string(Prompt, "$m", lower_case(mud_name())); 
    Prompt =replace_string(Prompt, "$N", (string)this_object()->GetCapName()); 
    Prompt =replace_string(Prompt, "$n", GetKeyName());
} 

string query_cwd() { return CurrentWorkingDirectory; } 

string query_prev_wd() { return PreviousWorkingDirectory; } 

string GetPrompt() { return DEFAULT_PROMPT; }

int query_mp() { return 1; } 

int query_max_mp() { return 10; } 

int query_hp() { return 1; } 

int query_max_hp() { return 10; } 

int query_sp() { return 1; } 

int query_max_sp() { return 10; } 

string get_path() { return query_cwd(); }

varargs int GetInvis(object ob) { return 0; }

string GetKeyName() { return 0; }




