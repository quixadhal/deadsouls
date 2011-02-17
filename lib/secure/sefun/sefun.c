/*    /secure/obj/sefun.c
 *    from Dead Souls
 *    the mud sefun object
 *    created by Descartes of Borg 940213
 */

#define MAX_DUMMIES 8192
#define MAX_OBJECT 1024

#include <lib.h>
#include <dirs.h>
#include <cfg.h>
#include <daemons.h>
#include <commands.h>
#include <objects.h>
#include <privs.h>
#include <runtime_config.h>
#include "./sefun.h"

#include "/secure/sefun/absolute_value.c"
#include "/secure/sefun/names.c"
#include "/secure/sefun/communications.c"
#include "/secure/sefun/copy.c"
#include "/secure/sefun/domains.c"
#include "/secure/sefun/economy.c"
#include "/secure/sefun/english.c"
#include "/secure/sefun/events.c"
#include "/secure/sefun/expand_keys.c"
#include "/secure/sefun/files.c"
#include "/secure/sefun/format_page.c"
#include "/secure/sefun/get_object.c"
#include "/secure/sefun/identify.c"
#include "/secure/sefun/interface.c"
#include "/secure/sefun/light.c"
#include "/secure/sefun/load_object.c"
#include "/secure/sefun/log_file.c"
#include "/secure/sefun/messaging.c"
#include "/secure/sefun/morality.c"
#include "/secure/sefun/mud_info.c"
#include "/secure/sefun/ordinal.c"
#include "/secure/sefun/parse_objects.c"
#include "/secure/sefun/path_file.c"
#include "/secure/sefun/percent.c"
#include "/secure/sefun/persist.c"
#include "/secure/sefun/pointers.c"
#include "/secure/sefun/query_time_of_day.c"
#include "/secure/sefun/absolute_path.c"
#include "/secure/sefun/security.c"
#include "/secure/sefun/strings.c"
#include "/secure/sefun/this_agent.c"
#include "/secure/sefun/time.c"
#include "/secure/sefun/to_object.c"
#include "/secure/sefun/translate.c"
#include "/secure/sefun/users.c"
#include "/secure/sefun/visible.c"
#include "/secure/sefun/tail.c"
#include "/secure/sefun/sockets.c"
#include "/secure/sefun/local_time.c"
#include "/secure/sefun/get_livings.c"
#include "/secure/sefun/get_verbs.c"
#include "/secure/sefun/get_cmds.c"
#include "/secure/sefun/get_stack.c"
#include "/secure/sefun/timestamp.c"
#include "/secure/sefun/duplicates.c"
#include "/secure/sefun/reaper.c"
#include "/secure/sefun/custom_path.c"
#include "/secure/sefun/mappings.c"
#include "/secure/sefun/dummy.c"
#include "/secure/sefun/disable.c"
#include "/secure/sefun/make_workroom.c"
#include "/secure/sefun/query_invis.c"
#include "/secure/sefun/rooms.c"
#include "/secure/sefun/generic.c"
#include "/secure/sefun/arrays.c"
#include "/secure/sefun/reload.c"
#include "/secure/sefun/wipe_inv.c"
#include "/secure/sefun/numbers.c"
#include "/secure/sefun/inventory.c"
#include "/secure/sefun/findobs.c"
#include "/secure/sefun/ascii.c"
#include "/secure/sefun/wild_card.c"
#include "/secure/sefun/compare_array.c"
#include "/secure/sefun/legacy.c"
#include "/secure/sefun/atomize.c"
#ifdef LIVEUPGRADE_SERVER
#include "/secure/sefun/native_version.c"
#endif
#include "/secure/sefun/minimap.c"
#include "/secure/sefun/fuzzymatch.c"
#include "/secure/sefun/astar.c"

object globalob;
string globalstr;
mixed globalmixed, gargs, gfun, gdelay;
int last_regexp = time();
int regexp_count = 1;
int max_regexp = 200;
private static string *blacklist = ({});
private static string *jokes = ({"bind","call_out","call_other",
        "unguarded","evaluate"});

#ifdef __FLUFFOS__
mixed copy(mixed val){
    return efun::copy(val);
}

string base_name(mixed val){
    if(!val) return "";
    return efun::base_name(val);
}
#endif /* __FLUFFOS__ */

varargs object clone_object(string name, mixed args...){
    int obsnum;
    string prev;

    prev = (base_name(previous_object()) || "");

    if(strsrch(prev,"/realms/") && strsrch(prev,"/open/")){
        return efun::clone_object(name, args...);
    }

    if(MEMUSE_SOFT_LIMIT && memory_info() > MEMUSE_SOFT_LIMIT){
        return 0;
    }

    if(last(name,2) == ".c") name = truncate(name,2);
    obsnum = sizeof( objects( (: base_name($1) == $(name) :) ) );
    //The following means "allow new dummies up to a total
    //of MAX_DUMMIES, but otherwise restrict the number of
    //clones created of a particular file by a non-privileged
    //object to MAX_OBJECT
    if((name != LIB_DUMMY && obsnum > MAX_OBJECT) || obsnum > MAX_DUMMIES){
        error("Too many cloned objects from an unprivileged source.");
    }
    return efun::clone_object(name, args...);
}

//Wodan removed saving binaries from FluffOS, causing a crasher
//if that config element is queried for in get_config().
mixed get_config(int i){
    if(i == 6) return 0;
    return efun::get_config(i);
}

//For some reason, FluffOS read_file() will read
//a zero-length file as a 65535 length T_INVALID variable.
//This tends to screw things up for Dead Souls.
varargs string read_file(string file, int start_line, int number_of_lines){
    int sz = file_size(file);
    if(sz < 1 || sz >= get_config(__MAX_STRING_LENGTH__)) return "";
    return efun::read_file(file, start_line, number_of_lines);
}

//In FluffOS 2.13 and early versions of 2.14, check_memory()
//is a crasher.
string check_memory(int flag){
    string ret;
#if 1
#ifdef __DEBUGMALLOC__
#ifdef __DEBUGMALLOC_EXTENSIONS__
#ifdef __PACKAGE_DEVELOP__
    ret = efun::check_memory(flag);
#endif
#endif
#endif
#endif
    if(!ret) ret = "";
    return ret;
}

mapping rusage(){
#ifdef __HAS_RUSAGE__
    return efun::rusage();
#else
    return ([ "nivcsw" : 0, "isrss" : 0, "nsignals" : 0, "utime" : 0,
            "oublock" : 0, "maxrss" : 0, "stime" : 0, 
            "nvcsw" : 0, "majflt" : 0, "nswap" : 0, "msgrcv" : 0, 
            "inblock" : 0, "minflt" : 0, "ixrss" : 0,
            "msgsnd" : 0, "idrss" : 0 ]);
#endif
}

string dump_file_descriptors(){
    if(!this_player() || !archp(this_player())){
        return "";
    }
    return efun::dump_file_descriptors();
}

void reset_eval_cost(){
    if(master()->valid_apply(({ "SECURE", "ASSIST" })))
        efun::reset_eval_cost();
}

void set_eval_limit(int i){
    if(master()->valid_apply(({ "SECURE", "ASSIST" })))
        efun::set_eval_limit(i);
}

string debug_info(int debuglevel, mixed arg){
    if(master()->valid_apply(({ "SECURE", "ASSIST" })))
        return efun::debug_info(debuglevel, arg);
    else return "This sefun is not available to unprivileged objects.";
}

string array groups(){
    string *group_arr = ({});
    string raw = read_file(CFG_GROUPS);
    string *raw_arr = explode(raw,"\n");
    foreach(string element in raw_arr){
        string s1,s2,s3;
        if(element[0..0] == "#"){
            continue;
        }
        if(sscanf(element,"(%s)%s",s1,s2) < 1)
            sscanf(element,"%s(%s)%s",s2,s1,s3);
        if(s1) group_arr += ({ s1 });
    }
    return singular_array(group_arr);
}

varargs string socket_address(mixed arg, int foo){
    return efun::socket_address(arg, foo);
}

varargs int call_out(mixed fun, mixed delay, mixed args...){
    object prev = previous_object();
    string prevbase, wat;
    mapping callers = ([]);
    string *raw = call_out_info();
    int i = sizeof(raw);
    mixed ret;

    gargs = args;
    gfun = fun;
    gdelay = delay;

    get_garbage();
    if(prev) prevbase = base_name(prev);
    else error("call_out with no previous_object()");

    if(sizeof(raw) > MAX_CALL_OUTS && (strsrch(prevbase,"/secure/") && 
                strsrch(prevbase,"/lib/") && strsrch(prevbase,"/std/") &&
                strsrch(prevbase,"/obj/") &&
                strsrch(prevbase,"/daemon/") && strsrch(prevbase,"/domains/"))){
        int err;
        globalmixed = prev;
        err = catch(unguarded( (: destruct( globalmixed ) :) ));
        error(prevbase+": Too many callouts!");
    }

    foreach(string foo in blacklist){
        if(!strsrch(prevbase,foo)){
            error("Object on call_out blacklist.");
        }
    }

    if(stringp(fun)){
        if(member_array(fun,jokes) != -1){
            error("No jokes, please!");
        }
        globalmixed = prev;
        fun = bind( (: call_other, globalmixed, gfun  :) ,prev);
    }
    gfun = fun;

    if(!strsrch(prevbase,"/open/")) error("call_out from /open");

    if(strsrch(prevbase,"/secure/") && strsrch(prevbase,"/daemon/")){
#if CALL_OUT_LOGGING
        unguarded( (: write_file("/log/secure/callouts",timestamp()+" "+
                        identify(previous_object(-1))+" "+identify((gargs || gfun))+"\n") :) );
#endif

        while(i--){
            if(sizeof(raw[i]) && objectp(raw[i][0])){
                string *lol;
                string wut;
                lol = explode(base_name(raw[i][0]),"/");
                wut = "/"+lol[0]+"/"+lol[1]+"/";
                if(!(callers[wut])) callers[wut] = 1;
                else callers[wut]++;
            }
        }
        wat = "/"+explode(prevbase,"/")[0]+"/"+explode(prevbase,"/")[1];
        if(callers[wat] && callers[wat] > 10){
            if(!strsrch(prevbase,REALMS_DIRS) || callers[wat] > 100){
                globalmixed = prev;
                blacklist += ({ wat });
                blacklist = singular_array(blacklist);
                unguarded( (: destruct(globalmixed) :) );
                error("Too many callouts from an unprivileged directory.");
            }
        }
    }

    if(sizeof(gargs)){
        ret = efun::call_out(gfun, gdelay, gargs...);
    }
    else {
        ret = efun::call_out(gfun, gdelay);
    }
    return ret;
}

string query_ip_number(object ob){
    string ret;
    if(!ob) ob = previous_object();
    ret = ob->GetTeloptIp();
    if(!ret) ret = efun::query_ip_number(ob);
    if(!AUTO_WIZ || ob == previous_object()) return ret;
    if(master()->valid_apply(({ "SECURE", "ASSIST" })))
        return ret;
    return "0.0.0.0";
}

//addr_server calls don't work well on Solaris and spam stderr
string query_ip_name(object ob){
    if(!ob) ob = previous_object();
    if(!strsrch(architecture(), "Solaris") ||
            ob->GetTeloptIp()) return query_ip_number(ob);
    if(!AUTO_WIZ || ob == previous_object()) return efun::query_ip_name(ob);
    if(master()->valid_apply(({ "SECURE", "ASSIST" })))
        return efun::query_ip_name(ob);
    return "w.x.y.z";
}

string *query_local_functions(mixed arg){
    object ob;
    string *allfuns;
    string *ret = ({}); 
    if(objectp(arg)) ob = arg;
    else if(stringp(arg)) ob = load_object(arg);
    allfuns = functions(ob);
    foreach(string subfun in allfuns){
        mixed thingy = function_exists(subfun,ob,1);
        if(thingy && thingy == base_name(ob) && member_array(subfun,ret) == -1) 
            ret += ({ subfun });
    }
    return ret;
}

object find_object( string str ){
    object ret;
    int err;
    string thing;
    if(!str || !stringp(str)) return 0;
    err = catch(ret = efun::find_object(str));
    if(err || !ret) return 0;
    thing = base_name(ret);
    if(!strsrch(thing, "/domains/")) return ret;
    if(base_name(previous_object()) == SERVICES_D) return ret;
    if(master()->valid_apply(({ "SECURE", "ASSIST", "SNOOP_D" }))) return ret;
    if(base_name(ret) == "/secure/obj/snooper") return 0;
    if(archp(ret) && ret->GetInvis()) return 0;
    else return ret;
} 

object find_player( string str ){
    object ret = efun::find_player(str);
    if(ret && !ret->GetInvis()) return ret;
    if(base_name(previous_object()) == SERVICES_D) return ret;
    if(master()->valid_apply(({ "SECURE", "ASSIST", "SNOOP_D" }))) return ret;
    if(ret && archp(ret) && ret->GetInvis()) return 0;
    else return ret;
}

object *livings() {
    object *privlivs = efun::livings();
    object *unprivlivs = ({});
#ifdef __FLUFFOS__
    unprivlivs = filter(privlivs, (: !($1->GetInvis() && archp($1)) :) );
#else
    foreach(mixed dude in privlivs){
        if(archp(dude) && dude->GetInvis()) continue;
        unprivlivs += ({ dude });
    }
#endif
    if(master()->valid_apply(({ "SECURE", "ASSIST", "SNOOP_D" }))) return privlivs;
    if(base_name(previous_object()) == SERVICES_D) return privlivs;
    else return unprivlivs;
}

varargs mixed objects(mixed arg1, mixed arg2){
    object array tmp_obs;

    if(!strsrch(base_name(previous_object()),"/secure/")){
        if(base_name(previous_object()) == "/secure/obj/weirder"){
            if(!arg1) return efun::objects();
            return efun::objects(arg1);
        }
        if(this_player() && adminp(this_player())){
            if(!arg1) return efun::objects();
            return efun::objects(arg1);
        }
    }

    if(arg1) tmp_obs = efun::objects(arg1);
    else tmp_obs = efun::objects();

    if(!(master()->valid_apply(({ "SECURE", "ASSIST", "SNOOP_D" }))) &&
            base_name(previous_object())  != SERVICES_D){
#ifdef __FLUFFOS__
        tmp_obs = filter(tmp_obs, (: !($1->GetInvis() && archp($1)) :) );
#else
        foreach(mixed dude in tmp_obs){
            if(dude && archp(dude) && dude->GetInvis()) tmp_obs -= ({ dude });
        }
#endif
    }
    if(base_name(previous_object()) == SNOOP_D || archp(this_player())){
        return tmp_obs;
    }
    if(!arg1){
        return filter(tmp_obs, (: base_name($1) != "/secure/obj/snooper" :) );
    }

    if(arg1 && !arg2) {
        object *ret_arr = ({});
        if(!functionp(arg1)) return 0;
        foreach(object ob in filter(tmp_obs, (: base_name($1) != "/secure/obj/snooper" :) )){
            if(evaluate(arg1, ob)) ret_arr += ({ ob });
        }
        return ret_arr;
    }

    if(arg1 && arg2) {
        object *ret_arr = ({});
        if(!functionp(arg1)) return 0;
        if(!objectp(arg2)) return 0;
        foreach(object ob in filter(tmp_obs, (: base_name($1) != "/secure/obj/snooper" :) )){
            if(call_other(arg2, arg1, ob)) ret_arr += ({ ob });
        }
        return ret_arr;
    }

    else return ({});
}

#ifdef __FLUFFOS__
mixed array users(){
    object *ret = filter(efun::users(), (: ($1) && environment($1) :) );
    if(!(master()->valid_apply(({ "SECURE", "ASSIST", "SNOOP_D" }))) &&
            base_name(previous_object())  != SERVICES_D)
        ret = filter(ret, (: !($1->GetInvis() && archp($1)) :) );
    return ret;
}
#else
mixed array users(){
    object *ret = ({});
    if(sizeof(efun::users()))
        foreach(mixed foo in efun::users()){
            if(objectp(foo) && environment(foo)) ret += ({ foo });
        }
    if(!(master()->valid_apply(({ "SECURE", "ASSIST", "SNOOP_D" }))) &&
            base_name(previous_object())  != SERVICES_D)
        foreach(mixed foo in ret){
            if(foo->GetInvis() && archp(foo)) ret -= ({ foo });
        }
    return ret;
}
#endif

int destruct(object ob) {
    string *privs;
    string tmp;
    int ok;
    if(!ob) return 0;
    privs = ({ file_privs(file_name(ob)) });

    if(!previous_object() || previous_object() == master() ||
            previous_object() == this_object()) ok = 1;

    else if(master()->valid_apply(({ "ASSIST" }) + privs)) ok = 1;

    else if(previous_object(0) && tmp = query_privs(previous_object(0))){
        if(previous_object(0) == ob) ok = 1;
        if(member_array(PRIV_SECURE, explode(tmp, ":")) != -1) ok = 1;
    }

    if(DESTRUCT_LOGGING){
        string stat = "FAIL";
        if(ok) stat = "success";
        log_file("destructs",timestamp()+"\n"+stat+"\n"+get_stack(1)+"\n--\n");
    }
    if(ob == this_object()){
        if( !(master()->valid_apply(({ "SECURE" }))) )
            error("Illegal attempt to destruct SEFUN: "+get_stack()+" "+identify(previous_object(-1)));
    }
    if(ok) return efun::destruct(ob);
    else return 0;
}

static void shutdown_logic(int code){
    efun::shutdown(code);
}

varargs void shutdown(int code) {
    object *persistents;
    object portals = find_object(PORTAL_D);
    object rooms = find_object(ROOMS_D);
    object players = find_object(PLAYERS_D);
    if(!(master()->valid_apply(({"ASSIST"}))) &&
            !(master()->valid_apply(({"SECURE"})))) return;
    if(code == -9) efun::shutdown(code);
    else call_out( (: shutdown_logic :), 0, code);
    if(this_player())
        log_file("shutdowns", this_player()->GetCapName()+
                " shutdown "+mud_name()+" at "+ctime(time())+"\n");
    else log_file("shutdowns", "Game shutdown by "+
            file_name(previous_object(0))+" at "+ctime(time())+"\n");
    persistents = objects( (: $1->GetPersistent() :) );
    if(portals) catch( portals->eventDestruct() );
    if(rooms) catch( rooms->eventDestruct() );
    if(players) catch( players->eventDestruct() );
    persistents->SaveObject();
}

int valid_snoop(object snooper, object target){
    if(member_group(target, PRIV_SECURE)) {
        message("system", snooper->GetCapName()+" is trying to snoop "
                "you.", target);
        if(!member_group(snooper, PRIV_SECURE)) return 0;
    }
    if(archp(snooper)) return 1;
    if( base_name(snooper) == "/secure/obj/snooper" ) return 1;
    //Uncomment the following line to let cres snoop players
    //if(creatorp(snooper) && playerp(target)) return 1; 
    if(snooperp(snooper) && creatorp(snooper) && playerp(target)) return 1;
    return 0;
}

varargs object snoop(object who, object target) {
    if(!target) return efun::snoop(who);
    if(!creatorp(who) && base_name(who) != "/secure/obj/snooper" ) return 0;
    if(!(master()->valid_apply(({ "ASSIST" })))) {
        if(!(target->query_snoopable())) return 0;
        else return efun::snoop(who, target);
    }
    else if(member_group(target, PRIV_SECURE)) {
        message("system", who->GetCapName()+" is now snooping "
                "you.", target);
        return efun::snoop(who, target);
    }
    else return efun::snoop(who, target);
}

    object query_snoop(object ob) {
        if(base_name(previous_object()) != SNOOP_D)
            return 0;
        return efun::query_snoop(ob);
    }

object query_snooping(object ob) {
    if(!(master()->valid_apply(({})))) return 0;
    else return efun::query_snooping(ob);
}

int exec(object target, object src) {
    string tmp;
    int ret;
    tmp = base_name(previous_object());
    if(tmp != LIB_CONNECT && tmp != CMD_ENCRE && tmp != CMD_DECRE 
            && tmp != SU && tmp != RELOAD_D) return 0;
    if(objectp(target) && objectp(src)) ret = efun::exec(target, src);
    return ret;
}

void write(string str) {
    if(this_player()) message("my_action", str, this_player());
    else efun::write(str);
}

void set_privs(object ob, string str) { return; }

void notify_fail(string str) {
    if( !this_player() ) return;
    if( str[<1..] == "\n" ) str = str[0..<2];
    this_player()->SetCommandFail(str);
}

/* want to handle colours, but do it efficiently as possible */
string capitalize(mixed str) {
    string *words, *tmp;
    int i;

    if(objectp(str)) str = str->GetKeyName();

    /* error condition, let it look like an efun 
     * mmmm let's not
     * if( !str || str == "" ) return efun::capitalize(str);
     */
    if(!str) str = "";
    /* most strings are not colour strings */
    if( strlen(str) < 2 || str[0..1] != "%^" ) return efun::capitalize(str);
    /* god help us */
    words = explode(str, " ");
    /* ok, this is strange, but remember, colours are all caps :) */
    tmp = explode(words[0], "%^");
    for(i=0; i<sizeof(tmp); i++) tmp[i] = efun::capitalize(tmp[i]);
    words[0] = "%^" + implode(tmp, "%^") + "%^";
    return implode(words, " ");
}

string *efuns(){
    return sort_array(MASTER_D->GetEfuns(),1);
}

string *sefuns(){
    return sort_array(functions(this_object()),1);
}

int efun_exists(string str){
    if(member_array(str,MASTER_D->GetEfuns()) != -1) return 1;
    return 0;
}

int sefun_exists(string str){
    if(member_array(str,functions(this_object())) != -1) return 1;
    return 0;
}

#if efun_defined(query_charmode)
varargs int in_input(object ob){
    int ret;
    if(ob) globalob = ob;
    else globalob = previous_object();
    if(!(efun::query_charmode(globalob)) &&
            !(globalob->GetProperty("was_charmode")) &&
            !(globalob->GetCharmode())){
        if(efun::in_input(globalob)) ret = 2;
    }
    return (ret ? 1 : 0);
}

int query_charmode(object ob){
    if(ob) globalob = ob;
    else globalob = previous_object();
    return efun::query_charmode(globalob);
}

#else
int query_charmode(object ob){
    return -1;
}
#endif

int in_pager(object ob){
    if(ob) globalob = ob;
    else globalob = previous_object();
    if(in_edit(globalob) || in_input(globalob)) return 1;
    if(globalob->GetProperty("was_charmode") && 
      !query_charmode(globalob)){
    //Ok, a bit counterintuitive, but the deal is that if you WERE in
    //charmode, and aren't anymore, the only reason for this is that
    //charmode is temporarily suspended so you can navigate a pager.
        return 1;
    }
    return 0;
}

varargs void input_to(mixed fun, int flag, mixed args...){
    object prev = previous_object();
    object player = this_player();
    int was;

    gargs = args;
    gfun = fun;
    gdelay = flag;

    if(!prev){
        error("input_to with no previous_object()");
    }
    if(stringp(fun)){
        if(member_array(fun,jokes) != -1){
            error("Please, no jokes!");
        }
        globalmixed = prev;
        fun = bind( (: call_other, globalmixed, gfun  :) ,prev);
    }
    gfun = fun;

    if(player && player->GetCharmode()){
#if efun_defined(remove_get_char)
        remove_get_char(player);
        was = 1;
#endif
#if efun_defined(remove_charmode)
        remove_charmode(player);
        was = 1;
#endif
        player->SetProperty("was_charmode", 1);
    }
    if(sizeof(gargs)){
        efun::input_to(gfun, gdelay, gargs...);
    }
    else {
        efun::input_to(gfun, gdelay);
    }
}

/* Do not use this sefun directly. */
varargs mixed file_present_logic(string str, object ob, int i){
    int cloned;
    mixed *rets = ({});
    if(objectp(str) || strsrch(str,"/")){
        return efun::present(str, ob);
    }
    if(grepp(str,"#")) cloned = 1;
    foreach(mixed item in all_inventory(ob)){
        if(cloned && file_name(item) == str) rets += ({ item });
        else if(!cloned && base_name(item) == str) rets += ({ item });
    }
    if(i && sizeof(rets)) return rets;
    else if(sizeof(rets)) return rets[0];
    return 0; 
}

varargs mixed present_file(mixed str, mixed ob, int i){
    object ret, env, *rets = ({});
    if(!str) return 0;
    if(objectp(str)) str = base_name(str);
    if(ob){ 
        ret = file_present_logic(str, ob, i);
        if(ret) return ret;
    }
    else {
        ob = previous_object();
        if(ob){
            ret = file_present_logic(str, ob, i);
            if(ret) return ret;
            env = environment(ob);
            if(env) ret = file_present_logic(str, env, i);
            if(ret) return ret;
        }
    }
    return 0;
}

varargs mixed present_bonus(mixed str, mixed ob, int i){
    mixed thingies = ( filter( all_inventory(ob),
                (: $1->GetBonusName() :) ) || ({}) );
    thingies = filter(thingies,(: $1->GetBonusName() == $(str) :));
    if(!i && sizeof(thingies)) return thingies[0];
    else if(sizeof(thingies)) return thingies;
    return 0;
}

int shadow_unhooked(){
    object ob = previous_object();
    return !(inherits(LIB_SHADOW_HOOK, ob));
}
