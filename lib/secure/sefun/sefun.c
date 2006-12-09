/*    /secure/obj/sefun.c
 *    from Dead Souls
 *    the mud sefun object
 *    created by Descartes of Borg 940213
 */

#include <lib.h>
#include <daemons.h>
#include <commands.h>
#include <objects.h>
#include <privs.h>
#include "sefun.h"

#include "/secure/sefun/absolute_value.c"
#include "/secure/sefun/base_name.c"
#include "/secure/sefun/communications.c"
#include "/secure/sefun/convert_name.c"
#include "/secure/sefun/copy.c"
#include "/secure/sefun/distinct_array.c"
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
#include "/secure/sefun/user_exists.c"
#include "/secure/sefun/user_path.c"
#include "/secure/sefun/visible.c"
#include "/secure/sefun/tail.c"
#include "/secure/sefun/dump_socket_status.c"
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
#include "/secure/sefun/singular_array.c"
#include "/secure/sefun/reload.c"
#include "/secure/sefun/wipe_inv.c"
#include "/secure/sefun/numbers.c"
#include "/secure/sefun/query_carrying.c"
#include "/secure/sefun/findobs.c"
#include "/secure/sefun/query_names.c"
#include "/secure/sefun/ascii.c"
#include "/secure/sefun/wild_card.c"
#include "/secure/sefun/compare_array.c"
#include "/secure/sefun/legacy.c"

string globalstr;
mixed globalmixed;

varargs string socket_address(mixed arg, int foo){
    return efun::socket_address(arg, foo);
}

#if CALL_OUT_LOGGING
//This is ugly and should not be used except in cases of dire
//emergency when you can't figure out wtf is choking your mud.
//This *will* break a bunch of stuff. You were warned.
int call_out(mixed args...){
    if(strsrch(base_name(previous_object()),"/secure/")
      && strsrch(base_name(previous_object()),"/daemon/")){
	globalmixed = args;
	unguarded( (: write_file("/log/secure/callouts",timestamp()+" "+
	      identify(previous_object(-1))+" "+identify(globalmixed)+"\n") :) );
    }
    switch(sizeof(args)){
    case 2 : return efun::call_out(args[0],args[1]);
    case 3 : return efun::call_out(args[0],args[1],args[2]);
    case 4 : return efun::call_out(args[0],args[1],args[2],args[3]);
    case 5 : return efun::call_out(args[0],args[1],args[2],args[3],args[4]);
    case 6 : return efun::call_out(args[0],args[1],args[2],args[3],args[4],args[5]);
    case 7 : return efun::call_out(args[0],args[1],args[2],args[3],args[4],args[5],args[6]);
    case 8 : return efun::call_out(args[0],args[1],args[2],args[3],args[4],args[5],args[6],args[7]);
    case 9 : return efun::call_out(args[0],args[1],args[2],args[3],args[4],args[5],args[6],args[7],args[8]);
    case 10 : return efun::call_out(args[0],args[1],args[2],args[3],args[4],args[5],args[6],args[7],args[8],args[9]);
    default : return 0;
    }
}
#endif

//addr_server calls don't work well on Solaris and spam stderr
string query_ip_name(object ob){
    if(architecture() == "Solaris") return query_ip_number(ob);
    else return efun::query_ip_name(ob);
}

function functionify(string str){
    globalstr = str;
    return (: globalstr :);
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
    if((int)master()->valid_apply(({ "SECURE", "ASSIST", "SNOOP_D" }))) return efun::find_object(str);
    if(base_name(efun::find_object(str)) == "/secure/obj/snooper") return 0;
    else return efun::find_object(str);
}

varargs mixed objects(mixed arg1, mixed arg2){
    object array tmp_obs = efun::objects();

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

mixed array users(){
    return filter(efun::users(), (: ($1) && environment($1) :) );
}

int destruct(object ob) {
    string *privs;
    string tmp;

    if(previous_object(0) && previous_object(0) == ob) return efun::destruct(ob);
    if(!(tmp = query_privs(previous_object(0)))) return 0;
    if(member_array(PRIV_SECURE, explode(tmp, ":")) != -1)
	return efun::destruct(ob);
    privs = ({ file_privs(file_name(ob)) });
    if((int)master()->valid_apply(({ "ASSIST" }) + privs))
	return efun::destruct(ob);
    else return 0;
}

varargs void shutdown(int code) {
    if(!((int)master()->valid_apply(({"ASSIST"}))) &&
      !((int)master()->valid_apply(({"SECURE"})))) return;
    if(this_player())
	log_file("shutdowns", (string)this_player()->GetCapName()+
	  " shutdown "+mud_name()+" at "+ctime(time())+"\n");
    else log_file("shutdowns", "Game shutdown by "+
	  file_name(previous_object(0))+" at "+ctime(time())+"\n");
    efun::shutdown(code);
}

int valid_snoop(object snooper, object target){
    if(member_group(target, PRIV_SECURE)) {
	message("system", (string)snooper->GetCapName()+" is trying to snoop "
	  "you.", target);
	if(!member_group(snooper, PRIV_SECURE)) return 0;
    }
    if(archp(snooper)) return 1;
    if( base_name(snooper) == "/secure/obj/snooper" ) return 1;
    if(creatorp(snooper) && playerp(target)) return 1; 
    return 0;
}

varargs object snoop(object who, object target) {
    if(!target) return efun::snoop(who);
    if(!creatorp(who) && base_name(who) != "/secure/obj/snooper" ) return 0;
    if(!((int)master()->valid_apply(({ "ASSIST" })))) {
	if(!((int)target->query_snoopable())) return 0;
	else return efun::snoop(who, target);
    }
    else if(member_group(target, PRIV_SECURE)) {
	message("system", (string)who->GetCapName()+" is now snooping "
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
    if(!((int)master()->valid_apply(({})))) return 0;
    else return efun::query_snooping(ob);
}

int exec(object target, object src) {
    string tmp;
    int ret;
    //tc("target: "+identify(target),"cyan");
    //tc("src: "+identify(src),"cyan");
    tmp = base_name(previous_object());
    if(tmp != LIB_CONNECT && tmp != CMD_ENCRE && tmp != CMD_DECRE && tmp != SU) return 0;
    //return efun::exec(target, src);
    ret = efun::exec(target, src);
    //tc("target: "+identify(target),"white");
    //tc("src: "+identify(src),"white");
    return ret;
}

void write(string str) {
    if(this_player()) message("my_action", str, this_player());
    else efun::write(str);
}

object *livings() {
    return efun::livings() - (efun::livings() - objects());
}

void set_privs(object ob, string str) { return; }

void set_eval_limit(int x) {
    if(previous_object() != master()) return;
    efun::set_eval_limit(x);
}

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

    /* error condition, let it look like an efun */
    if( !str || str == "" ) return efun::capitalize(str);
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

