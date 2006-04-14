#include <lib.h>
#include <save.h>
#include <rooms.h>
#include <config.h>

inherit LIB_DAEMON;

string *snooped = ({});
object *snoopers = ({});
string *monitored  = ({}); 
mapping Watchers = ([]);
object *prevusers;

void eventLoadRogues();

static void create() {
    daemon::create();
    debug("SNOOP_D restarted.","red");
    if( file_size( SAVE_SNOOP __SAVE_EXTENSION__ ) > 0 )
	unguarded( (: restore_object, SAVE_SNOOP, 1 :) );
    SetNoClean(1);
    set_heart_beat(60);
    if(GLOBAL_MONITOR == 0 && sizeof(monitored)){
	call_out( (: eventLoadRogues :), 1);
    }
    snoopers = filter(objects(), (: base_name($1) == "/secure/obj/snooper" :) );
}

void eventLoadRogues(){
    foreach(string rogue in monitored) this_object()->CheckBot(rogue);
    snoopers = filter(objects(), (: base_name($1) == "/secure/obj/snooper" :) );
}

int AddSnooper(){
    if(base_name(previous_object()) == "/secure/obj/snooper" &&
      member_array(previous_object(), snoopers) == -1){
	snoopers += ({ previous_object() });
	unguarded( (: save_object, SAVE_SNOOP, 1 :) );
	return 1;
    }
    return 0;
}

int RemoveSnooper(){
    if(base_name(previous_object()) == "/secure/obj/snooper" &&
      member_array(previous_object(), snoopers) != -1){
	snoopers -= ({ previous_object() });
	unguarded( (: save_object, SAVE_SNOOP, 1 :) );
	return 1;
    }
    return 0;
}

int CheckBot(string str){
    object cloan, foo;
    int allset;
    string *immune;
    string name;

    //tc("str: "+str);

    foreach(object snoopbox in snoopers){
	if(snoopbox->GetSnooped() == str) foo = find_player(str);
    }

    //if(foo && environment(foo)) tc("environment("+identify(foo)+"): "+identify(environment(foo)));
    if(!foo && GLOBAL_MONITOR > 0){
	if(archp(find_player(str)) && GLOBAL_MONITOR == 2) return 0;
	//tc("cloning: ");
	cloan=new("/secure/obj/snooper");
	//tc("vloan: "+identify(cloan));
	cloan->eventStartSnoop(str);
	//tc("cloning: "+identify(cloan)+" to snoop "+str);
    }
    unguarded( (: save_object, SAVE_SNOOP, 1 :) );
    return 1;
}

void CheckSnooped(){
    object *users;
    users = users();
    if(users && users != prevusers ){
	foreach(object user in users){
	    CheckBot(lower_case(user->GetKeyName()));
	}
	prevusers = users;
    }
}

void heart_beat(){
    CheckSnooped();
}

void reset(){
    if(query_heart_beat(this_object()) < 1) set_heart_beat(60);
    CheckSnooped();
}

int GetSnoop(string target, string msg){
    //if(target != "cratylus") {
    //tc(target+" "+msg+"\n\n","green");
    //tc("snooper: "+identify(previous_object()));
    //}
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
    if(Watchers[target] && !sizeof(Watchers[target])) map_delete(Watchers, target);
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
    if(!archp(previous_object(2))){
	return 0;
    }
    tc("Watchers: "+identify(Watchers));
    tc("snoopers: "+identify(snoopers));
    tc("prevusers: "+identify(prevusers));
    return 1;
}

