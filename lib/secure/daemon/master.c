/*    /secure/daemon/master.c
 *    from the Dead Souls LPC Library
 *    the master object, responsible for security
 *    created by Descartes of Borg 940910
 *    error handling by Beek@The Idea Exchange 941004
 *    Version: @(#) master.c 1.7@(#)
 *    Last modified: 96/12/14
 */

#include <config.h>
#include <rooms.h>
#include <cfg.h>
#include <lib.h>
#include <objects.h>
#include <privs.h>
#include <dirs.h>
#include <save.h>
#include <daemons.h>
#include <commands.h>
#include <network.h>
#include <parser_error.h>
#include <message_class.h>
#include "master.h"

private static int ResetNumber;
private static object Unguarded;
private static string PlayerName, rlog;
private static object NewPlayer;
private static mapping Groups, ReadAccess, WriteAccess;
private static string *ParserDirs = ({ "secure", "verbs", "daemon", "lib", "spells" });

void create() {
    Unguarded = 0;
    NewPlayer = 0;
    PlayerName = 0;
    ResetNumber = 1;
    new_read();
    new_write();
    new_groups();
    call_out( (: eventReset :), TIME_TO_RESET );
}

void new_read() {
    mapping tmp;

    tmp = ([]);
    load_access(CFG_READ, tmp);
    ReadAccess = tmp;
}

void new_write() {
    mapping tmp;

    tmp = ([]);
    load_access(CFG_WRITE, tmp);
    WriteAccess = tmp;
}

void new_groups() {
    mapping tmp;

    tmp = ([]);
    load_access(CFG_GROUPS, tmp);
    Groups = tmp;
}

private static void load_access(string cfg, mapping ref) {
    string *lines;
    string file;

    if( !(file = read_file(cfg)) ) {
	error("Failed to find config file: "+cfg);
    }
    lines = filter(explode(file, "\n"), function(string line) {

	  if( !line || trim(line) == "" ) {
	      return 0;
	  }
	  if( line[0] == '#' ) {
	      return 0;
	  }
	  return 1;
	});
      foreach(string line in lines) {
	  string fl, ac;

	  if( sscanf(line, "(%s) %s", fl, ac) != 2 ) {
	      error("Error in loading config file " + cfg + ".");
	  }
	  ref[fl] = explode(ac, ":");
      }
  }

    void flag(string str) {
	string file, arg;
	int i, x;

	if(previous_object()) return;
	if(sscanf(str, "for %d", x) == 1) {
	    for(i=0; i<x; i++) {}
	    return;
	}
	if(sscanf(str, "call %s %s", file, arg)) {
	    write("Got "+(string)call_other(file, arg)+" back.\n");
	    return;
	}
	write("Master: unknown flag.\n");
    }

    string *epilog(int x) {
	string *lines, *files;
	string content;
	int i;

	if(!(content = read_file(CFG_PRELOAD))) return ({});
	i = sizeof(lines = explode(content, "\n"));
	files = ({});
	while(i--) {
	    if(!lines[i] || lines[i] == "" || lines[i][0] == '#') continue;
	    files += ({ lines[i] });
	}
	return files;
    }

    string privs_file(string file) {
	string nom = 0;

	if( !strsrch(file, DIR_PLAYERS) ) sscanf(file, DIR_PLAYERS "/%*s/%s",nom);
	else if( !strsrch(file, DIR_CRES) ) sscanf(file, DIR_CRES "/%*s/%s",nom);
	if( nom ) {
	    if( file == DIR_CRES + "/" + nom[0..0] + "/" + nom ) {
		string str, grp;

		str = nom;
		foreach( grp in keys(Groups) )
		if( member_array(nom, Groups[grp]) != -1) str = str + ":" + grp;
		return str;
	    }
	    else if( file == DIR_PLAYERS + "/" + nom[0..0] + "/" + nom ) 
		return nom;
	    else return 0;
	}
	return file_privs(file);
    }

    void preload(string str) {
	string err;
	int t;

	if( !file_exists(str + ".c") ) return;
	t = time();
	write("Preloading: " + str + "...");
	if( err = catch(call_other(str, "???")) )
	    write("\nGot error "+err+" when loading "+str+".\n");
	else {
	    t = time() - t;
	    write("("+(t/60)+"."+(t%60)+")\n");
	}
    }

    int valid_write(string file, object ob, string fun) {
	string *ok;

	if( ob == master() ) return 1;
	ok = match_path(WriteAccess, file);
	return check_access(ob, fun, file, ok, "write");
    }

    int valid_read(string file, object ob, string fun) {
	string *ok;

	if( ob == master() ) return 1;
	ok = match_path(ReadAccess, file);
	return check_access(ob, fun, file, ok, "read");
    }

    int valid_link(string from, string to) {
	object ob = previous_object();
	if(!interactive(ob)) ob = previous_object(1);
	if(!interactive(ob)) ob = previous_object(2);
	if(!interactive(ob)) ob = previous_object(3);
	if(!interactive(ob)) {return 0;}
	if(ob->GetForced()) { tell_player(ob,"Someone's fucking with you."); return 0;}
	if(!valid_write(from, ob, "link")) {return 0;}
	if(!valid_write(to, ob, "link")) {return 0;}
	return 1;
    }

    int valid_apply(string *ok) {
	return check_access(previous_object(1),0,previous_object(0), ok, "apply");
    }

    int check_access(object ob, string fun, mixed file, string *ok, string oper) {
	object *stack;
	string *privs;
	string priv;
	int i, privcheck;

	if( objectp(file) ) file = base_name(file);
	if( ok && sizeof(ok) && ok[0] == "all" ) return 1;
	if( Unguarded == ob ) {
	    string tmp;

	    if( (tmp = base_name(ob)) == LIB_PLAYER || tmp == LIB_CREATOR) {
		if( !PlayerName ) i = sizeof(stack = ({ob})+previous_object(-1));
		else if( file == DIR_PLAYERS+"/"+PlayerName[0..0]+"/"+
		  PlayerName + __SAVE_EXTENSION__ )
		    return 1;
		else if( file == DIR_CRES+"/"+PlayerName[0..0]+"/"+
		  PlayerName + __SAVE_EXTENSION__ )
		    return 1;
		else i = sizeof(stack = ({ ob }));
	    }
	    else if( tmp + __SAVE_EXTENSION__ == file ) return 1;
	    else i = sizeof(stack = ({ ob }));
	}
	else if(Unguarded && base_name(ob) == "/secure/sefun/sefun") {
	    if(Unguarded == previous_object(1))
		stack = ({ previous_object(1) });
	    else stack = ({ ob }) + previous_object(-1);
	}
	else i = sizeof(stack = previous_object(-1) + ({ ob }));
	while(i--) {
	    if(!stack[i] || stack[i] == this_object()) {
		continue;
	    }
	    if(file_name(stack[i]) == SEFUN) {
		continue;
	    }
	    if(!(priv = query_privs(stack[i]))) {
		return 0;
	    }
	    if(!ok && oper == "read") {
		continue;
	    }
	    privs = explode(priv, ":");
	    if(member_array(PRIV_SECURE, privs) != -1) {
		continue;
	    }
	    if(stringp(file) && member_array(file_privs(file), privs) != -1) {
		continue;
	    }
	    if(!ok && oper == "write") {
		if(userp(stack[i]) && check_user(stack[i], fun, file, oper))
		    continue;
		else return 0;
	    }
	    if(sizeof(privs & ok)) {
		continue;
	    }
	    if(userp(stack[i]) && check_user(stack[i], fun, file, oper)) continue;
	    if(userp(stack[i]) && check_domain(stack[i], fun, file,oper)) continue;
	    return 0;
	}
	return 1;
    }

    nomask static int check_user(object ob, string fun, string file, string oper) {
	string nom, tmp;
	int x;

	if( !sscanf(file, REALMS_DIRS "/%s", nom) ) return 0;
	if( sscanf(nom, "%s/%*s", tmp) ) nom = tmp;
	nom = user_path(nom)+"adm/access";
	if(file_size(nom+".c") < 0) return 0;
	catch(x = (int)call_other(nom, "check_access", ob, fun, file, oper));
	return x;
    }

    nomask static int check_domain(object ob, string fun, string file, string o) {
	string nom;
	int x;

	if( !sscanf(file, DOMAINS_DIRS+"/%s/%*s", nom) ) return 0;
	nom = DOMAINS_DIRS+"/"+nom+"/adm/access";
	if(file_size(nom+".c") < 0) return 0;
	catch(x = (int)call_other(nom, "check_access", ob, fun, file, o));
	return x;
    }

    object connect(int port) {
	object ob;
	string err;
	string file;

	file = LIB_CONNECT;
	if( err  = catch(ob = new(file)) ) {
	    write("It looks like someone is working on the user object.\n");
	    write(err);
	    destruct(ob);
	}
	return ob;
    }

    object compile_object(string str) {
	string nom, tmp, where, which;
	object ob;

	if(sscanf(str, REALMS_DIRS+"/%s/%*s", nom))
	    tmp = sprintf("%svirtual/server", user_path(nom));
	else if(sscanf(str, DOMAINS_DIRS+"/%s/%*s", nom))
	    tmp = sprintf("%s/%s/virtual/server", DOMAINS_DIRS, nom);
	else if(strsrch(str, ESTATES_DIRS) == 0)
	    tmp = sprintf("%s/adm/server", ESTATES_DIRS);
	else if(sscanf(str, DIR_PLAYERS+"/%*s/%s", nom)) {
	    if(!NewPlayer) return 0;
	    if((string)NewPlayer->GetKeyName() != nom) return 0;
	    PlayerName = nom;
	    ob = new(LIB_PLAYER);
	    if(file_size(str+__SAVE_EXTENSION__) > 0) ob->restore_player(nom);
	    else if(file_size(DIR_PLAYERS) != -2) mkdir(DIR_PLAYERS);
	    else if(file_size(DIR_PLAYERS+"/"+nom[0..0]) != -2)
		mkdir(DIR_PLAYERS+"/"+nom[0..0]);
	    ob->SetKeyName(nom);
	    PlayerName = 0;
	    return ob;
	}
	else if( sscanf(str, DIR_CRES+"/%*s/%s", nom) ) {
	    if(!NewPlayer) return 0;
	    if((string)NewPlayer->GetKeyName() != nom) return 0;
	    PlayerName = nom;
	    ob = new(LIB_CREATOR);
	    if(file_size(str+__SAVE_EXTENSION__) > 0) ob->restore_player(nom);
	    ob->SetKeyName(nom);
	    PlayerName = 0;
	    return ob;
	}
	if(file_size(tmp+".c") < 0) {
	    if(sscanf(str, "%s.%s", where, which) != 2) return 0;
	    if(sscanf(str, REALMS_DIRS+"/%s/%*s", nom))
		tmp = sprintf("%svirtual/%s_server", user_path(nom), which);
	    else if(sscanf(str, DOMAINS_DIRS+"/%s/%*s", nom))
		tmp = sprintf("%s/%s/virtual/%s_server", DOMAINS_DIRS, nom, which);
	    if(file_size(tmp+".c") < 0) return 0;
	    else return (object)call_other(tmp, "compile_object", where);
	}
	return (object)call_other(tmp, "compile_object", str);               
    }

    static void crash(string err) {
	write_file(DIR_LOGS "/crashes", 
	  mud_name() + " crashed " + ctime(time()) + " with error " + 
	  err+".\n");
	message("system", "Reality implosion!!!  Everyone duck!!!", users());
	message("system", "You are being forced to quit.", users());
	users()->cmdQuit();
    }

    int valid_bind(object binder, object old_owner, object new_owner) {
	if( binder == master() ) return 1;
	if( member_array(PRIV_SECURE, explode(query_privs(binder), ":")) != -1 )
	    return 1;
	return 0;
    }

    int valid_hide(object who) {
	string priv;

	if(!objectp(who)) return 0;
	if(environment(who) && hiddenp(environment(who))) return 1;
	if(!(priv = query_privs(who))) return 0;
	else return (member_array(PRIV_SECURE, explode(priv, ":")) != -1);
    }

    int valid_override(string file, string nom) { return (file == SEFUN); }

    int valid_save_binary(string str) { return 1; }

    int valid_shadow(object ob) {
	object targ = previous_object();
	return (!virtualp(targ) && !strsrch(file_name(targ), DIR_SHADOWS));
    }

    int valid_object(object ob) {
	string file, contents;

	file = file_name(ob);
	if(COMPAT_MODE){
	}
	contents = read_file(base_name(ob)+".c");
	if(strsrch(contents,"parse_add_rule") != -1 
	  || strsrch(contents, "SetRules") != -1) {
	    string prefix;
	    if(!sscanf(file,"/%s/%*s",prefix)) return 0;
	    if(member_array(prefix, ParserDirs) == -1) return 0;
	}

	if( !strsrch(file, DIR_TMP) ) return 0;
	else if( !strsrch(file, DIR_FTP) ) return 0;
	else if( !strsrch(file, DIR_LOGS) ) return 0;
	else if( !strsrch(file, DIR_SECURE_SAVE) ) return 0;
	else return 1;
    }

    int valid_socket(object ob, string fun, mixed *info) {
	object *obs;
	int port;
	string tmp;
	int i;

	if( info && sizeof(info) == 4 ) {
	    ob = info[1];
	    port = info[3];
	    if( port == PORT_ADMIN && ob != find_object(ADMIN_D) ) {
		return 0;
	    }
	    if( port == PORT_RCP && ob != find_object(REMOTE_D) ) {
		return 0;
	    }
	}
	i = sizeof(obs = previous_object(-1));
	while(i--) {
	    if( !obs[i] ) continue;
	    if( userp(obs[i]) ) continue;
	    if( !(tmp = query_privs(obs[i])) ) return 0;
	    if( !sizeof(explode(tmp, ":") &
		({ PRIV_SECURE, PRIV_MUDLIB, PRIV_CMDS, PRIV_GENERAL })) ) return 0;
	}
	return 1;
    }

    mixed apply_unguarded(function f) {
	object previous_unguarded;
	string err;
	mixed val;

	if(base_name(previous_object(0)) != SEFUN) {
	    error("Illegal unguarded apply.");
	    return 0;
	}
	previous_unguarded = Unguarded;
	Unguarded = previous_object(1);
	err = catch(val = evaluate(f) );
	Unguarded = previous_unguarded;
	if(err) error(err);
	return val; 
    }

    string error_handler(mapping mp, int caught) {
	string ret, file;

	ret = "---\n" + standard_trace(mp);
	if( caught ) write_file(file = "/log/catch", ret);
	else write_file(file = "/log/runtime", ret);
	if( this_player(1) && find_object(SEFUN) ) {
	    this_player(1)->SetLastError(mp);
	    if( creatorp(this_player(1)) ) {
		this_player(1)->eventPrint(ret + "Trace written to " + file,
		  MSG_SYSTEM);
	    }
	    else {
		if( !strsrch(file_name(this_player(1)), LIB_CONNECT) ) {
		    return "/log/login";
		}
		this_player()->eventPrint("A runtime error occurred.");
		CHAT_D->eventSendChannel("System", "error", "A runtime error "
		  "occurred to " + 
		  (string)this_player(1)->GetCapName()+".");
		rlog = "-----\n" +timestamp()+ ": "+(string)this_player(1)->GetCapName()+"\n";
		rlog += load_object("/secure/cmds/creators/dbxwhere")->cmd(this_player(1)->GetKeyName());
		rlog += flat_map(this_player()->GetLastError())+"\n-----\n";
		unguarded( (: write_file("/log/player_errors", rlog) :) );
	    }
	}
	return 0;
    }

    void log_error(string file, string msg) {
	string nom, tmp;

	if( file[0] != '/' ) {
	    file = "/" + file;
	}
	if( sscanf(file, REALMS_DIRS+"/%s/%s", nom, tmp) != 2  && 
	  sscanf(file, DOMAINS_DIRS+"/%s/%s", nom, tmp) != 2 ) 
	    sscanf(file, "/%s/%s", nom, tmp);
	if( !nom ) nom = "log";
	catch(write_file(DIR_ERROR_LOGS "/" + nom, timestamp()+" "+msg));
	if(msg && this_player(1) && creatorp(this_player(1))){
	    catch(tell_player(this_player(1),msg));
	}
    }

    varargs string standard_trace(mapping mp, int flag) {
	string ret;
	mapping *trace;
	int i,n;

	ret = mp["error"] + "Object: " + 
	trace_line(mp["object"], mp["program"], mp["file"], mp["line"]);
	ret += "\n";
	trace = mp["trace"];
	n = sizeof(trace);
	for (i=0; i<n; i++) {
	    if( flag ) ret += sprintf("#%d: ", i);
	    ret += sprintf("'%s' at %s", trace[i]["function"], 
	      trace_line(trace[i]["object"], trace[i]["program"], 
		trace[i]["file"], trace[i]["line"]));
	}
	return ret;
    }

    string trace_line(object obj, string prog, string file, int line) {
	string ret;
	string objfn = obj ? file_name(obj) : "<none>";

	ret = objfn;
	if( different(objfn, prog) ) ret += sprintf(" (%s)", prog);
	if( file != prog ) ret += sprintf(" at %s:%d\n", file, line);
	else ret += sprintf(" at line %d\n", line);
	return ret;
    }

    int different(string fn, string pr) {
	int tmp;

	sscanf(fn, "%s#%d", fn, tmp);
	fn += ".c";
	return (fn != pr) && (fn != ("/" + pr));
    }

    void master_log_file(string file, string msg) {
	if(file_name(previous_object()) != SEFUN) return;
	if(file_size(file) > MAX_LOG_SIZE) rename(file, file+"."+timestamp());
	write_file(file, msg);
    }

    string make_path_absolute(string file) {
	return absolute_path((string)this_player(1)->query_cwd(), file);
    }

    int player_exists(string nom) {
	string str;

	if( !nom ) return 0;
	str = DIR_PLAYERS "/" + nom[0..0] + "/" + nom + __SAVE_EXTENSION__;
	if( file_size(str) > -1 ) return 1;
	str = DIR_CRES "/" + nom[0..0] + "/" + nom + __SAVE_EXTENSION__;
	return (file_size(str) > -1);
    }

    string domain_file(string str) {
	string nom, tmp;

	if(sscanf(str, DOMAINS_DIRS+"/%s/%s", nom, tmp) == 2) return nom;
	return 0;
    }

    string author_file(string str) {
	string nom, tmp;

	if(sscanf(str, REALMS_DIRS+"/%s/%s", nom, tmp) == 2) return nom;
	return 0;
    }

    static int slow_shutdown() {
	write_file(DIR_LOGS "/audit", 
	  "Armageddon loaded by master: "+ctime(time())+".\n");
	EVENTS_D->eventRebootMud(2);
	return 1;
    }

    int save_ed_setup(object who, int code) {
	string file;

	if(!intp(code)) return 0;
	rm(file = user_path((string)who->GetKeyName())+".edrc");
	return write_file(file, code+"");
    }

    int retrieve_ed_setup(object who) {
	string file;

	file = user_path((string)who->GetKeyName())+".edrc";
	if(!file_exists(file)) return 0;
	return to_int(read_file(file));
    }

    string get_save_file_name(string file) {
	string str;

	str = (string)this_player(1)->GetKeyName();
	if(file_size(user_path(str)) == -2)
	    return user_path(str)+"dead.edit";
	else return DIR_TMP+"/"+str+".dead.edit";
    }

    int is_locked() { return MUD_IS_LOCKED; }

    string *parse_command_id_list() { return ({ "one", "thing" }); }

    string *parse_command_plural_id_list() { return ({ "ones", "things","them"}); }

    string *parse_command_adjectiv_id_list() {
	return ({ "the", "an", "a" });
    }

    string *parse_command_prepos_list() {
	return ({ "in", "with", "without", "into", "for", "on", "under", "against",
	  "out", "within", "of", "from", "between", "at", "to", "over", "near",
	  "inside", "onto", "off", "through", "across", "up", "down", "every",
	  "around", "about", "only", "here", "room", "exit", "enter", "-r", "-a"});
    }

    string parse_command_all_word() { return "all"; }

    string parser_error_message(int type, object ob, mixed arg, int flag) {
	string err;

	if( ob ) err = (string)ob->GetShort();
	else err = "";
	switch(type) {
	case ERR_IS_NOT:
	    if( flag ) err = "There is no such " + remove_article(arg) + " here.";
	    else err = "There is no " + remove_article(arg) + " here.";
	    break;

	case ERR_NOT_LIVING:
	    if( flag )
		err = "None of the " + pluralize(remove_article(arg)) +" are alive.";
	    else err = "The " + remove_article(arg) + " is not alive.";
	    break;

	case ERR_NOT_ACCESSIBLE:
	    if( flag ) err = "You can't get to them.";
	    else err = "You can't get to it.";
	    break;

	case ERR_AMBIG:
	    {
		mixed *obs;
		int i;

		obs = unique_array(arg, (: (string)$1->GetShort() :));
		if( sizeof(obs) == 1 )
		    err = "Which of the " +
		    consolidate(sizeof(arg), (string)obs[0][0]->GetShort()) +
		    " do you mean?";
		else {
		    err = "Do you mean ";
		    for(i = 0; i<sizeof(obs); i++) {
			if( sizeof(obs[i]) > 1 )
			    err += "one of the " +
			    consolidate(sizeof(obs[i]),(string)obs[i][0]->GetShort());
			else err += (string)obs[i][0]->GetShort();
			if( i == (sizeof(obs)-2) ) err += " or ";
			else if( i < sizeof(obs) - 1 ) err += ", ";
		    }
		    err += "?";
		}
		return err;
	    }

	case ERR_ORDINAL:
	    if( arg > 1 ) err = "There are only " + arg + " of them.";
	    else err = "There is only one of them.";
	    break;

	case ERR_ALLOCATED:
	    return arg;

	case ERR_THERE_IS_NO:
	    return "There is no " + remove_article(arg) + " here.";

	case ERR_BAD_MULTIPLE:
	    return "You can't do that to more than one at a time.";
	}
	return err;
    }

    void create_save() {
	string str;

	if(!stringp(str = (string)previous_object()->GetKeyName())) return;
	if(file_size(DIR_PLAYERS+"/"+str[0..0]) == -2) return;
	if(str[0] < 'a' || str[0] > 'z') return;
	mkdir(DIR_PLAYERS+"/"+str[0..0]);
    }

    object player_object(string nom) {
	object ob;
	string err, tmp;
	int old_limit;

	tmp = base_name(ob = previous_object());
	if( tmp != CMD_ENCRE && tmp != CMD_DECRE && tmp != LIB_CONNECT )
	    return 0;
	old_limit = max_eval_cost();
	set_eval_limit(1000000000);
	NewPlayer = ob;
	if(file_size(DIR_CRES+ "/" + nom[0..0]+ "/" +nom+__SAVE_EXTENSION__) > -1) 
	    err = catch(ob = load_object(DIR_CRES+"/"+nom[0..0]+"/"+nom));
	else err = catch(ob = load_object(DIR_PLAYERS+"/"+nom[0..0]+"/"+nom));
	NewPlayer = 0;
	set_eval_limit(old_limit);
	if(err) error(err);
	return ob;
    }

    string player_save_file(string nom) { 
	string tmp;

	tmp = DIR_CRES + "/" + nom[0..0] + "/" + nom;
	if( file_size(tmp + __SAVE_EXTENSION__) > -1 ) return tmp;
	else return DIR_PLAYERS + "/" + nom[0..0] + "/" + nom;
    }

    string *query_group(string grp) { return copy(Groups[grp]); }

    mapping query_groups() { return copy(Groups); }

    static void eventReset() {
	object *obs;
	object ob;
	int x, y;

	ResetNumber++;
	call_out( (: eventReset :), TIME_TO_RESET );
	x = reclaim_objects();
	write_file(DIR_LOGS "/reset", "Reset " + ResetNumber + " occurred at: " +
	  ctime(time()) + "\n");
	obs = objects( (: !environment($1) && (random(100) < 26) :) );
	y = 0;
	foreach(ob in obs) {
	    function f;

	    if( !ob ) {
		y++;
		continue;
	    }
	    f = bind( (: call_other, ob, "clean_up" :), ob );
	    if( f ) catch(evaluate(f));
	    if( !ob ) {
		y++;
		continue;
	    }
	    f = bind((: call_other, ob, "reset", ResetNumber :), ob);
	    if( f ) catch(evaluate(f));
	}
	write_file(DIR_LOGS "/reset", "\t" + x + " objects reclaimed, " +
	  (sizeof(obs) - y) + " objects reset, " + y + " objects "
	  "cleaned.\n");
    }

    int GetResetNumber() { return ResetNumber; }

    object *parse_command_users() {
	return filter(users(), (: creatorp($1) || (int)$1->is_living() :));
    }
