/*    /secure/daemon/master.c
 *    from the Dead Souls LPC Library
 *    the master object, responsible for security
 *    created by Descartes of Borg 940910
 *    error handling by Beek@The Idea Exchange 941004
 *    Version: @(#) master.c 1.7@(#)
 *    Last modified: 96/12/14
 */

#include <runtime_config.h>
#include ROOMS_H
#include <cfg.h>
#include <lib.h>
#include <objects.h>
#include <privs.h>
#include <dirs.h>
#include <save.h>
#include <daemons.h>
#include <commands.h>
#include NETWORK_H
#include <parser_error.h>
#include <message_class.h>
#include <function.h>
#include "master.h"

#ifndef COMPAT_MODE
#define COMPAT_MODE 1
#endif
#ifndef DEFAULT_PARSING
#define DEFAULT_PARSING 1
#endif
#ifndef RESET_ALL
#define RESET_ALL 1
#endif
#ifndef MAX_SINGLE_OBJECT
#define MAX_SINGLE_OBJECT        4000
#endif
#define CRAT_PARSE 0
#ifndef CONSOLE_TRACE
#define CONSOLE_TRACE 0
#endif
#define INST_DEBUG 0

inherit LIB_DAEMON;

private static int incept_date, ResetNumber, heart_count, in_reset;
private static int eval_threshold, reset_handle = -1;
private int PerformanceScore, globalpmsg;
private static object Unguarded, gguy;
private static string PlayerName, rlog, gcmd, bname, gstr;
private static object NewPlayer;
private static mapping Groups, ReadAccess, WriteAccess, CostErr;
private static string *ParserDirs = ({ "secure", "verbs", "daemon", "lib", "powers" });
private static string array efuns_arr = ({});
private static string SaveFile;

static void Setup(){
    SaveFile = save_file(SAVE_MASTER);
    if(!PerformanceScore && unguarded( (: file_exists(SaveFile) :) )){
        unguarded( (: RestoreObject(SaveFile) :) );
    }
    unguarded( (: SaveObject(SaveFile) :) );
}

void create() {
#ifdef __OPCPROF__
    string tmpfun, junk, opstr;
    string *oparr = ({});
    string opfile = "/tmp/opc";
    opcprof(opfile);
    if(file_exists(opfile+".efun")){
        opstr = read_file(opfile+".efun");
        if(opstr){ 
            oparr = explode(opstr, "\n");
        }
        if(sizeof(oparr)){
            foreach(string element in oparr){
                if(sscanf(element,"%s %s",tmpfun, junk) == 2){
                    if(tmpfun[0..0] == "_") tmpfun = tmpfun[1..];
                    efuns_arr += ({ tmpfun });
                }
            }
        }
    }
#endif
    eval_threshold = (get_config(__MAX_EVAL_COST__) / 1000000) + 1;
    incept_date = time();
    PerformanceScore = 0;
    Unguarded = 0;
    NewPlayer = 0;
    PlayerName = 0;
    ResetNumber = 1;
    if(!(file_exists("/secure/sefun/native_version.c"))){
        cp("/secure/scripts/native_version.proto", 
                "/secure/sefun/native_version.c");
    }
    new_read();
    new_write();
    new_groups();
    reset_handle = call_out( (: eventReset :), TIME_TO_RESET );
    call_out( (: Setup :), 10);
    set_heart_beat(1);
}

static void heart_beat(){
    heart_count++;
    if(!(heart_count % 60)){
        CostErr = ([]);
        if(in_reset){
            eventReset();
        }
    }
    if(heart_count > 64000) heart_count = 0;
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

private static void load_access(string cfg, mapping resource) {
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
        string fl, ac = "";

        if( sscanf(line, "(%s)%s", fl, ac) != 2 ) {
            error("Error in loading config file " + cfg + ".");
        }
        ac = trim(ac);
        if(last(fl,3) == "/*/"){
            string tmp = truncate(fl,2);
            foreach(string element in get_dir(tmp)){
                resource[tmp+element+"/"] = explode(ac, ":");
            }
        }

        else resource[fl] = explode(ac, ":");
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
        write("Got "+call_other(file, arg)+" back.\n");
        return;
    }
    write("Master: unknown flag.\n");
}

string *epilog(int nopreload) {
    string *lines, *files;
    string content;
    int i;

    if(nopreload || !(content = read_file(CFG_PRELOAD))) return ({});
    i = sizeof(lines = explode(content, "\n"));
    files = ({});
    while(i--) {
        if(!lines[i] || lines[i] == "" || lines[i][0] == '#') continue;
        files += ({ lines[i] });
    }
    return files;
}

string privs_file(string file) {
    string tmp, nom = 0;
    mixed ret;
    if( !strsrch(file, DIR_PLAYERS) ) sscanf(file, DIR_PLAYERS "/%*s/%s",tmp);
    else if( !strsrch(file, DIR_CRES) ) sscanf(file, DIR_CRES "/%*s/%s",tmp);
    if(tmp && grepp(tmp, ".")) nom = cleaned_name(tmp);
    else nom = tmp;
    //if(tmp || globalpmsg) debug_message("\n---->"+file+"<----");
    //if(tmp || globalpmsg) debug_message("---->"+tmp+"<----");
    //if(tmp || globalpmsg) debug_message("---->"+nom+"<----");
    if( nom ) {
        if(!grepp(file,".")) file += ".";
        if(!strsrch(file, DIR_CRES + "/" + nom[0..0] + "/" + nom + ".")){
            string str, grp;

            str = nom;
            foreach( grp in keys(Groups) )
                if( member_array(nom, Groups[grp]) != -1) str = str + ":" + grp;
            ret = str;
        }
        else if( !strsrch(file, DIR_PLAYERS + "/" + nom[0..0] + 
                    "/" + nom + "."))
            ret = nom;
        else ret = 0;
    }
    if(undefinedp(ret)) ret = file_privs(file);
    //if(tmp || globalpmsg) debug_message("---->"+ret+"<----\n");
    //if(!ret) debug_message("\n*\n!!!--->"+file+"<---!!!\n*\n");
    return ret;
}

void preload(string str) {
    string err;
    mapping before, after;
    int t;
#ifdef __HAS_RUSAGE__
    before = rusage();
#endif
    if( !file_exists(str = lpc_file(str)) ) return;
    write("Preloading: " + str + "...");

    if( err = catch(call_other(str, "???")) ){
        write("\nGot error "+err+" when loading "+str+".\n");
    }
    else {
#ifdef __HAS_RUSAGE__
        after = rusage();
        if(sizeof(before) && sizeof(after)){
            t = after["utime"] - before["utime"];
            PerformanceScore += t;
            write("("+t+"ms)\n");
        }            
        return;
#endif
        write("(done)\n");
    }
    unguarded( (: SaveObject(SaveFile) :) );
}

int valid_write(string file, object ob, string fun) {
    string *ok;
    int ret;
    if( ob == master() ) ret = 1;
    ok = match_path(WriteAccess, file);
    if(!ret) ret = check_access(ob, fun, file, ok, "write");
    if(INST_DEBUG && grepp(file, "ratylus")){
        tc("ok: "+identify(ok));
        tc("valid_write("+file+", "+identify(ob)+", "+fun+"): "+ret);
    }
    return ret;
}

int valid_read(string file, object ob, string fun) {
    string *ok;
    int ret;
    if( ob == master() ) ret = 1;
    ok = match_path(ReadAccess, file);
    if(!ret) ret = check_access(ob, fun, file, ok, "read");
    if(INST_DEBUG && !ret){
        debug_message("OK: "+identify(ok));
        debug_message("VALID_READ FAIL ("+file+", "+identify(ob)+", "+fun+"): "+ret);
        debug_message("\n===\nstack: "+get_stack()+"\n===\n");
    }
    return ret;
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
    int ret;
    ret = check_access(previous_object(1),0,previous_object(0), ok, "apply");
    if(INST_DEBUG && !ret){
        tc("FAILED VALID APPLY. ok: "+identify(ok)+", args: "+identify(previous_object(1))+", "+identify(previous_object(0)));
        tc("stack: "+get_stack());
    }
    return ret;
}

int check_access(object ob, string fun, mixed file, string *ok, string oper) {
    object *stack;
    string *privs;
    string priv, tmp;
    int i, dbg;

    if( objectp(file) ) tmp = base_name(file);
    if(tmp) file = tmp;
    if(INST_DEBUG){
        dbg = 1;
    }
    if(dbg){
        //tc("stack: "+get_stack(1));
        tc("CHECK_ACCESS file: "+file,"blue");
        tc("CHECK_ACCESS ok: "+identify(ok),"blue");
        tc("CHECK_ACCESS ob: "+identify(ob),"blue");
        tc("CHECK_ACCESS tmp: "+identify(tmp),"blue");
    }
    if( ok && sizeof(ok) && ok[0] == "all" ) return 1;
    if( Unguarded == ob ){
        tmp = base_name(ob);
        if(dbg) tc("tmp: "+tmp, "black");
        if( tmp == LIB_PLAYER || tmp == LIB_CREATOR){
            string sfilep, sfilec;
            if(dbg) tc("hi. wtf.","red");
            sfilep = DIR_PLAYERS+"/"+PlayerName[0..0]+"/"+ PlayerName + __SAVE_EXTENSION__;
            sfilec = DIR_CRES+"/"+PlayerName[0..0]+"/"+ PlayerName + __SAVE_EXTENSION__;
            if(dbg){
                tc("CHECK_ACCESS file: "+file,"green");
                tc("CHECK_ACCESS sfilep: "+sfilep,"green");
                tc("CHECK_ACCESS sfilec: "+sfilec,"green");
                tc("CHECK_ACCESS ob: "+identify(ob),"green");
            }
            if( !PlayerName ) i = sizeof(stack = ({ob})+previous_object(-1));
            else if( file == sfilep ) return 1;
            else if( file == sfilec ) return 1;
#if ENABLE_INSTANCES
            else if( file == new_savename(sfilep) ){
                return 1;
            }
            else if( file == new_savename(sfilec) ){
                return 1;
            }
#endif
            else i = sizeof(stack = ({ ob }));
        }
        else if( tmp == file ) return 1;
        else if( tmp + __SAVE_EXTENSION__ == file ) return 1;
#if ENABLE_INSTANCES
        else if( new_savename(tmp) == file ) return 1;
#endif
        else i = sizeof(stack = ({ ob }));
    }
    else if(Unguarded && base_name(ob) == SEFUN) {
        if(Unguarded == previous_object(1))
            stack = ({ previous_object(1) });
        else stack = ({ ob }) + previous_object(-1);
    }
    else i = sizeof(stack = previous_object(-1) + ({ ob }));
    while(i--) {
        if(dbg) tc("ooo stack["+i+"]: "+identify(stack[i]));
        if(!stack[i] || stack[i] == this_object()) {
            continue;
        }
        if(file_name(stack[i]) == SEFUN) {
            continue;
        }
        if(dbg) globalpmsg = 1;
        else globalpmsg = 0;
        if(!(priv = query_privs(stack[i]))) {
            if(false()){
                debug_message("\nSPLAT 1: "+identify(stack[i]));
            }    
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
            if(dbg) tc("hmm.","blue");
            if(userp(stack[i]) && check_user(stack[i], fun, file, oper)){
                continue;
            }
            else {
                if(false()) debug_message("\nSPLAT 2\n");
                return 0;
            }
        }
        if(dbg) tc("privs: "+identify(privs));
        if(sizeof(privs & ok)) {
            continue;
        }
        if(userp(stack[i]) && check_user(stack[i], fun, file, oper)) continue;
        if(userp(stack[i]) && check_domain(stack[i], fun, file,oper)) continue;
        if(dbg) tc("SADFACE","red");
        return 0;
    }
    return 1;
}

nomask static int check_user(object ob, string fun, string file, string oper){
    string nom, tmp;
    int x;
    if(interactive(ob) && !creatorp(ob)){
        nom = ob->GetKeyName();
        if(!strsrch(file,DIR_ESTATES + "/"+nom[0..0]+"/"+nom)) return 1;
    }
    if( !sscanf(file, REALMS_DIRS "/%s", nom) ) return 0;
    if( sscanf(nom, "%s/%*s", tmp) ) nom = tmp;
    nom = user_path(nom)+"adm/access";
    if(strsrch(nom,"/tmp") && file_size(nom+".c") > 0)
        catch(x = call_other(nom, "check_access", ob, fun, file, oper));
    return x;
}

nomask static int check_domain(object ob, string fun, string file, string o) {
    string nom;
    int x;
    if( !sscanf(file, DOMAINS_DIRS+"/%s/%*s", nom) ) return 0;
    nom = DOMAINS_DIRS+"/"+nom+"/adm/access";
    if(file_size(nom+".c") < 0) return 0;
    catch(x = call_other(nom, "check_access", ob, fun, file, o));
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
    string nom, tmp, where, which, sfile;
    object ob;
    //tc("HAI! compile_object: "+str, "red");
    sfile = str+__SAVE_EXTENSION__;
    if(sscanf(str, REALMS_DIRS+"/%s/%*s", nom))
        tmp = sprintf("%svirtual/server", user_path(nom));
    else if(sscanf(str, DOMAINS_DIRS+"/%s/%*s", nom))
        tmp = sprintf("%s/%s/virtual/server", DOMAINS_DIRS, nom);
    else if(strsrch(str, ESTATES_DIRS) == 0)
        tmp = sprintf("%s/virtual/server", ESTATES_DIRS);
    else if(sscanf(str, DIR_PLAYERS+"/%*s/%s", nom)) {
        //tc("playa");
        if(!NewPlayer){
            //tc("NOT NEW PLAYA RETURN WAT");
            return 0;
        }
#if ENABLE_INSTANCES
        nom = old_savename(nom);
#endif
        if(last(nom,2) == ".o") nom = truncate(nom, 2);
        if(last(sfile,4) == ".o.o") sfile = truncate(sfile, 2);
        //tc("playa2 "+nom+", "+NewPlayer->GetKeyName());
        if(NewPlayer->GetKeyName() != nom) return 0;
        //tc("playa3 sfile: "+sfile);
        PlayerName = nom;
        ob = new(LIB_PLAYER);
        //tc("sfile: "+sfile);
        //tc("new sfile: "+new_savename(sfile));
        if(file_exists(sfile) || file_exists(new_savename(sfile))){ 
            //tc("booyah");
            ob->restore_player(nom);
            //tc("ob: "+identify(ob));
        }
        else {
            //tc("omg WAT WAT WAT");
            if(file_size(DIR_PLAYERS) != -2) mkdir(DIR_PLAYERS);
        }
        if(file_size(DIR_PLAYERS+"/"+nom[0..0]) != -2)
            mkdir(DIR_PLAYERS+"/"+nom[0..0]);
        ob->SetKeyName(nom);
        PlayerName = 0;
        //tc("ob: "+identify(ob));
        return ob;
    }
    else if( sscanf(str, DIR_CRES+"/%*s/%s", nom) ) {
        if(!NewPlayer) return 0;
#if ENABLE_INSTANCES
        nom = old_savename(nom);
        if(last(nom,2) == ".o") nom = truncate(nom, 2);
#endif
        if(last(sfile,4) == ".o.o") sfile = truncate(sfile, 2);
        if(NewPlayer->GetKeyName() != nom) return 0;
        PlayerName = nom;
        ob = new(LIB_CREATOR);
        //tc("sfile: "+sfile);
        //tc("new sfile: "+new_savename(sfile));
        if(file_exists(sfile) || file_exists(new_savename(sfile))){
            //tc("WIIIN");
            ob->restore_player(nom);
        }
        else {
            //tc("FAAALEEEE");
        }
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
        else return call_other(tmp, "compile_object", where);
    }
    return call_other(tmp, "compile_object", str);               
}

static void crash(mixed args...) {
    string err;
    string guilty_stack = get_stack(1);
    string guilty_obs = identify(previous_object(-1));
    if(sizeof(args)) err = args[0];
    write_file(DIR_LOGS "/crashes",
            mud_name() + " crashed " + ctime(time()) + " with error " +
            err+".\n"+guilty_stack+"\n"+guilty_obs+"\n---\n");
    message("system", "Reality implosion!!!  Everyone duck!!!", users());
    message("system", "You are being forced to quit.", users());
    users()->cmdQuit();
}

int valid_bind(object binder, object old_owner, object new_owner) {
    int ret;
    mixed privs;
    if(!binder) ret = 0;
    else if( binder == this_object() ) ret = 1;
    else if( base_name(binder) == SEFUN ) ret = 1;
    else if( member_array(PRIV_SECURE, 
                explode((privs = query_privs(binder)), ":")) != -1 ) ret = 1;
    //if(!ret) tc("valid_bind("+identify(binder)+", "+identify(old_owner)+", "+identify(new_owner)+"): "+ret+" (privs: "+identify(privs)+"), stack: "+get_stack());
    return ret;
}

int valid_hide(object who) {
    string priv;
    if(!objectp(who)) return 0;
    if(environment(who) && hiddenp(environment(who))) return 1;
    if(!(priv = query_privs(who))) return 0;
    else return (member_array(PRIV_SECURE, explode(priv, ":")) != -1);
}

int valid_override(string file, string nom){ 
    return (file == SEFUN);
}

int valid_save_binary(string str) { return 1; }

int valid_shadow(object ob) {
    object targ = previous_object();
    return (!virtualp(targ) && !strsrch(file_name(targ), DIR_SHADOWS));
}

int valid_object(object ob) {
    string file, contents;
    file = file_name(ob);
    contents = read_file(base_name(ob)+".c");
    if(!contents) contents = "";
    if(strsrch(contents,"parse_add_rule") != -1 
            || strsrch(contents, "SetRules") != -1) {
        string prefix;
        if(!sscanf(file,"/%s/%*s",prefix)) return 0;
        if(member_array(prefix, ParserDirs) == -1) return 0;
    }
    if( !strsrch(file, DIR_TMP) ) return 0;
    else if( !strsrch(file, DIR_FTP) ) return 0;
    else if( !strsrch(file, DIR_LOGS) ) return 0;
    else if( !strsrch(file, DIR_SECURE_SAVE) ){
        return 0;
    }
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
        tmp = query_privs(obs[i]);
        if(!tmp && base_name(obs[i]) == SEFUN) tmp = "SECURE";
        if( !(tmp) ) return 0;
        if( !sizeof(explode(tmp, ":") &
                    ({ PRIV_SECURE, PRIV_MUDLIB, PRIV_CMDS, PRIV_GENERAL })) ){
            return 0;
        }
    }
    return 1;
}

mixed apply_unguarded(function f) {
    object previous_unguarded;
    string err;
    mixed val;

    if(!f || !functionp(f)){
        error("Invalid function passed.");
        return 0;
    }

    if((functionp(f) & FP_OWNER_DESTED)){
        error("Function owner dested: invalid function.");
        return 0;
    }

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
    string ret, file, dbg;
    int now = time();

    if(!CostErr) CostErr = ([ now : 0 ]);
    else if(undefinedp(CostErr[now])) CostErr[now] = 0;

    if(CONSOLE_TRACE){
        dbg = "\n----\n";
        foreach(mixed key, mixed val in mp){
            dbg += identify(key) + " : "+identify(val)+"\n";
        }
        dbg += "\n----\n";
        debug_message(dbg);
    }
    if(mp && mp["error"]){
        if(grepp(mp["error"], "Too deep recursion")){
            debug_message("Too deep recursion\n"+get_stack());
        }
        if(grepp(mp["error"], "Too many open files")){
            debug_message("Too many open files!\n");
            shutdown(-9);
        }
        if(grepp(mp["error"], "Too long evaluation.")){
            CostErr[now]++; 
        }
        if(grepp(mp["error"], "Can't catch eval cost too big error.")){
            CostErr[now]++;
        }
        if(CostErr[now] >= eval_threshold){
            /* This prevents a strange eval cost cascade that
             * can sometimes happen under extreme load. Unfortunately
             * the only effective countermeasure is a hard reboot
             * of the mud.
             */
            debug_message("Eval cost error cascade!\n");
            shutdown(-9);
        }
    }
    ret = "---\n"+timestamp()+"\n"+ standard_trace(mp);
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
                return "/log/login\n"+standard_trace(mp)+"\n--\n";
            }
            this_player()->eventPrint("A runtime error occurred.");
            CHAT_D->eventSendChannel("System", "error", "A runtime error "
                    "occurred to " + 
                    this_player(1)->GetCapName()+".");
            rlog = "-----\n" +timestamp()+ ": "+this_player(1)->GetCapName()+"\n";
            rlog += load_object("/secure/cmds/creators/dbxwhere")->cmd(this_player(1)->GetKeyName());
            rlog += flat_map(this_player()->GetLastError())+"\n-----\n";
            unguarded( (: write_file("/log/player_errors", rlog) :) );
        }
    }
    return 0;
}

void log_error(string file, string msg) {
    string nom, tmp, tmp2;

    if( file[0] != '/' ) {
        file = "/" + file;
    }
    if(!this_player()){
        object *object_stack = call_stack(1);
        object web_sessions = load_object(WEB_SESSIONS_D);
        if(web_sessions && member_array(web_sessions, object_stack) != -1){
            web_sessions->ReceiveErrorReport(msg);
        }
    }
    if( sscanf(file, REALMS_DIRS+"/%s/%s", nom, tmp) != 2 && 
            sscanf(file, DOMAINS_DIRS+"/%s/%s", nom, tmp) != 2 && 
            sscanf(file, ESTATES_DIRS+"/%s/%s/%s", tmp, nom, tmp2) != 3 ) 
        sscanf(file, "/%s/%s", nom, tmp);
    if( !nom ) nom = "log";
    catch(write_file(DIR_ERROR_LOGS "/" + nom, timestamp()+" "+msg));
    if(msg && this_player(1) && builderp(this_player(1))){
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
    return absolute_path(this_player(1)->query_cwd(), file);
}

int player_exists(string nom) {
    string sfilec, sfilep;
    if( !nom ) return 0;
    sfilec = DIR_CRES "/" + nom[0..0] + "/" + nom + __SAVE_EXTENSION__;
    sfilep = DIR_PLAYERS "/" + nom[0..0] + "/" + nom + __SAVE_EXTENSION__;
#if ENABLE_INSTANCES
    sfilep = new_savename(sfilep);
    sfilec = new_savename(sfilec);
#endif
    if(unguarded((: file_exists, sfilec :))) return 1;
    if(unguarded((: file_exists, sfilep :))) return 1;
    return 0;
}

string domain_file(string str) {
    string nom, tmp;
    if(sscanf(str, DOMAINS_DIRS+"/%s/%s", nom, tmp) == 2) return nom;
    return 0;
}

string author_file(string str) {
    string nom, tmp, tmp2;

    if(sscanf(str, REALMS_DIRS+"/%s/%s", nom, tmp) == 2) return nom;
    else if(sscanf(str, ESTATES_DIRS+"/%s/%s/%s", tmp, nom, tmp2) == 3) return nom;
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
    rm(file = user_path(who->GetKeyName())+".edrc");
    return write_file(file, code+"");
}

int retrieve_ed_setup(object who) {
    string file;

    file = user_path(who->GetKeyName())+".edrc";
    if(!file_exists(file)) return 0;
    return to_int(read_file(file));    }

    string get_save_file_name() {
        mixed str;

        if(!this_player(1)) return DIR_TMP+"/.dead.edit";
        str = this_player(1)->GetKeyName();
        if(!str || !stringp(str)) return DIR_TMP+"/.dead.edit";
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
    object tmpob;
#if CRAT_PARSE
    debug("parser_error_message("+
        TYPES_D->eventCalculateTypes("parser_error",type)+", "+
        identify(ob)+", "+
        identify(arg)+", "+
        identify(flag)+") "
            ,"yellow");
    debug("type: "+type,"green");
#endif
    if( ob ) err = ob->GetShort();
    else err = "";
    switch(type) {
        string wut;
        object wat;

        case 0:
#if CRAT_PARSE
        debug("WTF. An unhandled error?","red");
#endif
        if(arg && objectp(arg)) wat = arg;
        if(!wat && arg && arrayp(arg) && sizeof(arg)){
            foreach(mixed element in arg){
                if(objectp(element)) wat = element;
            }
        }
        if(ob && wat){
            err = "You can't use "+ob->GetShort()+" with "+
                wat->GetShort()+" that way.";
        }
        else if(ob){
            err = "It seems you can't do that with " +ob->GetShort()+".";
        }
        else if(wat){
            err = "It seems you can't do that to " +wat->GetShort()+".";
        }
        else {
            err = "It seems you can't do that.";
        }
        break;

        case ERR_IS_NOT:
#if CRAT_PARSE
        debug(identify(this_player())+" ERR_IS_NOT: "+identify(ob)+", "+identify(arg)+", "+identify(flag),"green");
#endif
        if(flag || (arg && stringp(arg))){
            if(flag || get_object(arg, this_player())){
                return "It appears you must be more specific.";
            }
            else if(arg && stringp(arg)) wut = remove_article(arg);
        }
        else wut = "that";
        err = capitalize(wut) +" is not here.";
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
            gguy = this_player();
            if(DEFAULT_PARSING){
                gcmd = this_player()->GetLastCommand();
                this_player()->eventRetryCommand(gcmd, type, arg);
                return " ";
            }

            obs = unique_array(arg, (: $1->GetShort() :));
            if( sizeof(obs) == 1 )
                err = "Which of the " +
                    consolidate(sizeof(arg), obs[0][0]->GetShort()) +
                    " do you mean?";
            else {
                err = "Do you mean ";
                for(i = 0; i<sizeof(obs); i++) {
                    if( sizeof(obs[i]) > 1 )
                        err += "one of the " +
                            consolidate(sizeof(obs[i]),obs[i][0]->GetShort());
                    else err += obs[i][0]->GetShort();
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
#if CRAT_PARSE
        debug(identify(this_player())+" ERR_THERE_IS_NO: "+identify(ob)+", "+identify(arg)+", "+identify(flag),"blue");
#endif
        if(flag || (arg && stringp(arg)) && environment(this_player())){
            if(tmpob = present(arg, environment(this_player()))){
                return "It seems you must be more specific.";
            }
            else if(arg && stringp(arg)) wut = remove_article(arg);
        }
        else wut = "such thing";
        err = "There is no "+ wut +" here.";
        break;

        case ERR_BAD_MULTIPLE:
        err = "You can't do that to more than one at a time.";
        break;
    }
    return err;
}

void create_save() {
    string str;
    if(!stringp(str = previous_object()->GetKeyName())) return;
    if(file_size(DIR_PLAYERS+"/"+str[0..0]) == -2) return;
    if(str[0] < 'a' || str[0] > 'z') return;
    mkdir(DIR_PLAYERS+"/"+str[0..0]);
}

varargs object player_object(string nom, object stub) {
    object ob;
    string err, tmp;
    int old_limit;
    string sfilec = DIR_CRES+ "/" + nom[0..0]+ "/" +nom+__SAVE_EXTENSION__;
    string sfilep = DIR_PLAYERS+ "/" + nom[0..0]+ "/" +nom+__SAVE_EXTENSION__;
    tmp = base_name(ob = previous_object());
    if( tmp != CMD_ENCRE && tmp != CMD_DECRE && tmp != LIB_CONNECT 
            && tmp != RELOAD_D ){
        tc("BOOOO!");
        return 0;
    }
    old_limit = max_eval_cost();
    set_eval_limit(1000000000);
    if(tmp == RELOAD_D && stub) NewPlayer = stub;
    else NewPlayer = ob;
#if ENABLE_INSTANCES
    if(file_exists(new_savename(sfilec))){
        err = catch(ob = load_object(new_savename(sfilec)));
        //tc("1: "+sfilec+": err: "+err+", ob: "+identify(ob));
    }
    else{
        err = catch(ob = load_object(new_savename(sfilep)));
        //tc("2: "+sfilep+": err: "+err+", ob: "+identify(ob));
    }
#else
    if(ob == previous_object() && file_exists(sfilec)){
        string tmpy;
        if(last(sfilec,2) == ".o") tmpy = truncate(sfilec,2);
        ob = load_object(tmpy);
        //tc("3: "+tmpy+": err: "+err+", ob: "+identify(ob));
        if(!ob) ob = load_object(sfilec);
        //tc("4: "+tmpy+": err: "+err+", ob: "+identify(ob));
    }
    if(!ob || ob == previous_object()){
        string tmpy;
        if(last(sfilep,2) == ".o") tmpy = truncate(sfilep,2);
        ob = load_object(tmpy);
        //tc("5: "+tmpy+": err: "+err+", ob: "+identify(ob));
        if(!ob) ob = load_object(sfilep);
        //tc("6: "+sfilep+": err: "+err+", ob: "+identify(ob));
    }
#endif
    if(!ob || ob == previous_object()){
        //tc("7: WAT");
    }
    NewPlayer = 0;
    set_eval_limit(old_limit);
    if(err) error(err);
    return ob;
}

string player_save_file(string nom) { 
    string sfilec, sfilep;
    sfilec = DIR_CRES + "/" + nom[0..0] + "/" + nom + ".o";
    sfilep = DIR_PLAYERS + "/" + nom[0..0] + "/" + nom + ".o";
#if ENABLE_INSTANCES
    gstr = new_savename(sfilec);
    if(unguarded( (: file_exists, gstr :) )){ 
        return gstr;
    }
    return new_savename(sfilep);
#endif
    if(unguarded( (: file_exists, sfilec :) ) ){
        //tc("player_save_file "+nom+": VANILLA CRE");
        return sfilec;
    }
    //tc("player_save_file "+nom+": VANILLA PLAY");
    return sfilep;
}

string *query_group(string grp) { return copy(Groups[grp]); }

mapping query_groups() { return copy(Groups); }

static void eventReset(){
    object *obs;
    object ob;
    int x, y;
    in_reset = 1;

    ResetNumber++;
    if(find_call_out("eventReset") == -1 && find_call_out(reset_handle) == -1){ 
        reset_handle = call_out( (: eventReset :), TIME_TO_RESET );
    }
    x = reclaim_objects();
    write_file(DIR_LOGS "/reset", "Reset " + ResetNumber + " occurred at: " +
            ctime(time()) + "\n");
    if(!RESET_ALL) obs = objects( (: !environment($1) && (random(100) < 26) :) );
    else obs = objects( (: !environment($1) :) );
    if(sizeof(obs) > 500){
        obs = scramble_array(obs)[0..500];
    }
    y = 0;
    foreach(ob in obs) {
        function f;

        if(ob) f = bind( (: call_other, ob, "clean_up" :), ob );
        else {
            continue;
        }
        if( f ) catch(evaluate(f));
        if( !ob ) {
            y++;
            continue;
        }
        f = bind((: call_other, ob, "reset", ResetNumber :), ob);
        if( f ) catch(evaluate(f));
    }
    in_reset = 0;
    write_file(DIR_LOGS "/reset", "\t" + x + " objects reclaimed, " +
            (sizeof(obs) - y) + " objects reset, " + y + " objects "
            "cleaned.\n");
}

int RequestReset(){
    if(previous_object() && (base_name(previous_object()) == SEFUN 
                || base_name(previous_object()) == REAPER_D)){
        unguarded( (: eventReset :) );
        return 1;
    }
    return 0;
}

int GetResetNumber() { return ResetNumber; }

string *GetEfuns(){
    return (({})+ efuns_arr);
}

object *parse_command_users() {
    return filter(users(), (: creatorp($1) || $1->is_living() :));
}

int GetPerformanceScore(){
    return PerformanceScore;
}

int GetPerfOK(){
    if(!PerformanceScore && uptime() > 60 &&
            (time() - incept_date) < 5) return 0;
    if(PerformanceScore <= MIN_PERF) return 1;
    return 0;
}
