#include <lib.h>
#include <config.h>

inherit LIB_DAEMON;
mapping Reloadees = ([]);
string savefile = "/secure/save/reload";

static void create() {
    daemon::create();
    if(!file_exists(savefile+__SAVE_EXTENSION__)) save_object(savefile);
    else restore_object(savefile);
    set_heart_beat(1);
}

varargs int eventReload(mixed what, int when){
    //tc("received thing","cyan");
    if(!when) when = time();
    else when += time();
    if(!what) return 0;
    if(stringp(what)){
	if(last(what,2) == ".c") what = trim(what,2);
	what = find_object(what);
	if(!what) return 0;
    }
    if(!objectp(what)) return 0;
    if(Reloadees[what]) return 0;
    if( (previous_object() != what) && 
      !((int)master()->valid_apply(({ "ASSIST" }))) ){
	log_file("adm/reload_d",get_stack()+" "+identify(previous_object(-1))+
	  " attempted to use RELOAD_D: "+timestamp()+"\n");
	tell_creators(get_stack()+" "+identify(previous_object(-1))+
	  " attempted to use RELOAD_D: "+timestamp()+"\n");
	error("Illegal attempt to access RELOAD_D: "+get_stack()+" "+identify(previous_object(-1)));
    }
    what = file_name(what);
    Reloadees[what] = when;
}

void heart_beat(){
    foreach(mixed key, mixed val in Reloadees){
	if(time() >= val){
	    map_delete(Reloadees,key);
	    tc("bink: "+key);
	    key = find_object(key);
	    tc("bink2: "+identify(key));
	    reload(key);
	}
    }
}

mapping GetReloads(){
    return copy(Reloadees);
}

mapping ClearReloads(){
    if(!((int)master()->valid_apply(({ "ASSIST" }))) ){
	log_file("adm/reload_d",get_stack()+" "+identify(previous_object(-1))+
	  " attempted to clear RELOAD_D: "+timestamp()+"\n");
	tell_creators(get_stack()+" "+identify(previous_object(-1))+
	  " attempted to clear RELOAD_D: "+timestamp()+"\n");
	error("Illegal attempt to access RELOAD_D: "+get_stack()+" "+identify(previous_object(-1)));
    }
    Reloadees = ([]);
    save_object(savefile);
    return copy(Reloadees);
}

int eventDestruct(){
    save_object(savefile);
    return daemon::eventDestruct();
}
