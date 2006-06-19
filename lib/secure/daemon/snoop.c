#include <lib.h>
#include <privs.h>
#include <save.h>
#include <rooms.h>
#include <config.h>

inherit LIB_DAEMON;

int SnoopClean();

string *snooped = ({});
object *snoopers = ({});
string *monitored  = ({}); 
mapping Watchers = ([]);
int count = 0;
int just_loaded = 1;
object *prevusers;

void eventLoadRogues();

static void create() {
    daemon::create();
    debug("SNOOP_D restarted.","red");
    if( file_size( SAVE_SNOOP __SAVE_EXTENSION__ ) > 0 )
	unguarded( (: restore_object, SAVE_SNOOP, 1 :) );
    SetNoClean(1);
    SnoopClean();
    set_heart_beat(1);
    if(GLOBAL_MONITOR == 0 && sizeof(monitored)){
	call_out( (: eventLoadRogues :), 1);
    }
    snoopers = filter(objects(), (: base_name($1) == "/secure/obj/snooper" :) );
    prevusers = users();
}

void RegisterSnooper(){
    object registrant = previous_object();
    if(base_name(registrant) == "/secure/obj/snooper" &&
      member_array(registrant,snoopers) == -1) snoopers += ({ registrant });
    unguarded( (: save_object, SAVE_SNOOP, 1 :) );
}

void UnregisterSnooper(){
    object registrant = previous_object();
    if(base_name(registrant) == "/secure/obj/snooper" &&
      member_array(registrant,snoopers) != -1) snoopers -= ({ registrant });
    unguarded( (: save_object, SAVE_SNOOP, 1 :) );
}

void eventLoadRogues(){
    if( !((int)master()->valid_apply(({ PRIV_SECURE }))) ) return;
    foreach(string rogue in monitored) this_object()->CheckBot(rogue);
    snoopers = filter(objects(), (: base_name($1) == "/secure/obj/snooper" :) );
}

int CheckBot(string str){
    object cloan, foo;
    int allset, already_watched = 0;
    string *immune;
    string name;

    if(!str) str = "foo";
    str = lower_case(str);
    foo = find_player(str);
    if(sizeof(snoopers)){
	foreach(object snoopbox in snoopers){
	    if(snoopbox && grepp("#",file_name(snoopbox)) ) {
	    }
	    else snoopers -= ({snoopbox});
	    if(snoopbox && snoopbox->GetSnooped() && snoopbox->GetSnooped() == str) {
		already_watched = 1;
	    }
	    if(snoopbox->GetSnooped() && (!find_player(snoopbox->GetSnooped()) ||!(snoopbox->GetSnooped()))) {
	    }
	}
    }
    if(!already_watched && foo && (GLOBAL_MONITOR > 0 || member_array(str, monitored) != -1 || member_array(str, snooped) != -1 )){
	if(archp(find_player(str)) && GLOBAL_MONITOR == 2) return 0;
	//tc("already watched: "+already_watched);
	cloan=new("/secure/obj/snooper");
	//tc("cloning : "+identify(cloan));
	cloan->eventStartSnoop(str);
    }
    //tc("query_snooping: "+identify(query_snooping(cloan)));
    unguarded( (: save_object, SAVE_SNOOP, 1 :) );
    return 1;
}

void CheckSnooped(){
    object *lusers = users();
    if(!compare_array(lusers, prevusers) || just_loaded){
	just_loaded = 0;
	foreach(object user in lusers){
	    CheckBot(user->GetKeyName());
	}
    }
    else CheckBot("tanstaafl");
    prevusers = lusers;
}

int SnoopClean(){
    snoopers = filter(objects(), (: base_name($1) == "/secure/obj/snooper" :) );
    if(sizeof(snoopers)){
	foreach(object snoopbox in snoopers){
	    if(snoopbox && !clonep(snoopbox) ) {
		snoopers -= ({snoopbox});
	    }
	    else {
		object subject;
		string dude = snoopbox->GetSnooped();
		if(dude) subject = find_player(dude);
		if(!dude || !subject || !query_snooping(snoopbox) || 
		  (member_array(dude,snooped) == -1 && member_array(dude,monitored) == -1 && GLOBAL_MONITOR < 1 )){
		    snoopbox->eventDestruct();
		    snoopers -= ({snoopbox});
		}
	    }
	}
    }
    return 1;
}

void heart_beat(){
    count++;

    //tc("snoop daemon heart_beat","green");
    if( !(count % 5) ) CheckSnooped();

    if( !(count % 10)) {
	foreach(object snoopbox in snoopers){
	    if(!snoopbox) snoopers -= ({ snoopbox });
	}
	SnoopClean();
    }
    if(count > 100) count = 0;
}

void reset(){
    //tc("Snoop daemon reset");
    if(query_heart_beat(this_object()) < 1) set_heart_beat(60);
    CheckSnooped();
}

int GetSnoop(string target, string msg){
    if(target != "cratylus") {
    }
    if(base_name(previous_object()) != "/secure/obj/snooper") return 0;
    else if(sizeof(Watchers[target])) {
	foreach(string watcher in Watchers[target]){
	    tell_player(watcher, target+"% "+msg);
	}
    }
    else Watchers[target] = ({});
    return 1;
}

int AddWatcher(string watcher, string target){
    if(!valid_snoop(find_player(watcher), find_player(target))) {
	return 0;
    }
    if(Watchers[target] && member_array(watcher,Watchers[target]) != -1){
	return 0;
    }
    if(!Watchers[target] || !sizeof(Watchers[target])) {
	Watchers[target] = ({ watcher });
    }
    else {
	Watchers[target] += ({ watcher });
    }
    if(member_array(target,snooped) == -1) snooped += ({target});
    CheckBot(target);
    unguarded( (: save_object, SAVE_SNOOP, 1 :) );
    return 1;
}

int RemoveWatcher(string watcher, string target){
    if(!valid_snoop(find_player(watcher), find_player(target))) return 0;
    if(this_player() && interactive(this_player())){
	if(this_player()->GetKeyName() != watcher && !archp(this_player())) return 0;
    }
    else return 0;

    if(!Watchers[target] || !sizeof(Watchers[target])) return 0;
    else if(member_array(watcher, Watchers[target]) != -1) 
	Watchers[target] -= ({ watcher });
    if((!Watchers[target] || !sizeof(Watchers[target])) &&
      member_array(target, monitored) == -1) {
	foreach(object snoopbox in snoopers){
	    if(snoopbox->GetSnooped() == target) snoopbox->eventDestruct(); 
	}
    }
    if(!Watchers[target] || !sizeof(Watchers[target])) {
	snooped -= ({target});
	if(Watchers[target]) map_delete(Watchers, target);
    }
    CheckBot("adsfgrertgrsgnfxmy");
    unguarded( (: save_object, SAVE_SNOOP, 1 :) );
    return 1;
}

int AddMonitor(string requestor, string target){
    if(member_array(target, monitored) == -1) monitored += ({ target });
    unguarded( (: save_object, SAVE_SNOOP, 1 :) );
    CheckBot(target);
    return 1;
}

int RemoveMonitor(object requestor, string target){
    if(!archp(requestor)) return 0;
    monitored -= ({ target });
    unguarded( (: save_object, SAVE_SNOOP, 1 :) );
    if(Watchers[target] && sizeof(Watchers[target])) return 1;
    foreach(object snoopbox in snoopers){
	if(snoopbox->GetSnooped() == target) snoopbox->eventDestruct(); 
    }
    if(Watchers[target] && !sizeof(Watchers[target])) map_delete(Watchers, target);
    CheckBot("asreg54eqwhtrbsf");
    unguarded( (: save_object, SAVE_SNOOP, 1 :) );
    return 1;
}

int ReportLinkDeath(string str){
    if(!find_player(str)){
	foreach(object snoopbox in snoopers){
	    if(snoopbox->GetSnooped() == str) snoopbox->eventDestruct(); 
	}
	map_delete(Watchers, str);
	unguarded( (: save_object, SAVE_SNOOP, 1 :) );
	return 1;
    }
    return 0;
}

int ReportReconnect(string str){
    if(base_name(previous_object()) == LIB_CONNECT){
	foreach(object snoopbox in snoopers){
	    if(snoopbox->GetSnooped() == str) snoopbox->eventDestruct(); 
	}
	map_delete(Watchers, str);
	CheckBot(str);
	unguarded( (: save_object, SAVE_SNOOP, 1 :) );
	return 1;
    }
    return 0;
}

int Report(){
    if( !((int)master()->valid_apply(({ "PRIV_SECURE" }))) ){
	return 0;
    }
    tc("Watchers: "+identify(Watchers));
    tc("snoopers: "+identify(snoopers));
    tc("prevusers: "+identify(prevusers));
    tc("snooped: "+identify(snooped));
    tc("monitored: "+identify(monitored));
    return 1;
}

