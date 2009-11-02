#include <lib.h>
#include <daemons.h>
#include <privs.h>
#include <save.h>
#include ROOMS_H

inherit LIB_DAEMON;

int SnoopClean();
int RemoveWatcher(string watcher, mixed target);

string *snooped = ({});
object *snoopers = ({});
string *monitored  = ({}); 
string *ignored = ({});
static string *ignored_ips = ({"97.107.133.86","149.158.218.102","204.209.44.3","24.34.143.119","69.37.84.233", "127.0.0.1"});
mapping Watchers = ([]);
int count = 0;
int just_loaded = 1;
object *prevusers, gfoo;
static string SaveFile;

void eventLoadRogues();

static void create() {
    daemon::create();
    if(!ignored) ignored = ({});
    SaveFile = save_file(SAVE_SNOOP);
    if(file_exists(SaveFile)){
        RestoreObject(SaveFile, 1);
    }
    SetNoClean(1);
    SnoopClean();
    set_heart_beat(1);
    if(GLOBAL_MONITOR == 0 && sizeof(monitored)){
        call_out( (: eventLoadRogues :), 1);
    }
    snoopers = filter(objects(), (: base_name($1) == "/secure/obj/snooper" :) );
    prevusers = users();
    catch(mkdir("/secure/log/adm/archive"));
}

void RegisterSnooper(){
    object registrant = previous_object();
    if(base_name(registrant) == "/secure/obj/snooper" &&
            member_array(registrant,snoopers) == -1) snoopers += ({ registrant });
    SaveObject(SaveFile, 1);
}

void UnregisterSnooper(){
    object registrant = previous_object();
    if(base_name(registrant) == "/secure/obj/snooper" &&
            member_array(registrant,snoopers) != -1) snoopers -= ({ registrant });
    SaveObject(SaveFile, 1);
}

void eventLoadRogues(){
    if( !(master()->valid_apply(({ PRIV_SECURE }))) ) return;
    foreach(string rogue in monitored) this_object()->CheckBot(rogue);
    snoopers = filter(objects(), (: base_name($1) == "/secure/obj/snooper" :) );
}

int CheckBot(string str){
    object cloan, foo;
    int allset, err, already_watched = 0;
    string *immune;
    string name;

    if(!str) str = "foo";
    str = lower_case(str);
    foo = find_player(str);
    if(sizeof(snoopers)){
        foreach(object snoopbox in snoopers){
            if(clonep(snoopbox) ) {
            }
            else snoopers -= ({snoopbox});
            if(snoopbox && snoopbox->GetSnooped() && snoopbox->GetSnooped() == str) {
                already_watched = 1;
            }
        }
    }
    if(!already_watched && foo && (GLOBAL_MONITOR > 0 || member_array(str, monitored) != -1 || member_array(str, snooped) != -1 )){
        string ip;
        gfoo = foo;
        ip = unguarded( (: query_ip_number(gfoo) :) );
        gfoo = 0;
        if(member_array(str, (ignored || ({}))) != -1) return 0;
        if((member_array(ip, (ignored_ips||({}))) != -1 ||
                    (archp(foo) && GLOBAL_MONITOR == 2)) &&
                member_array(str, monitored) == -1 &&
                member_array(str, snooped) == -1 ) return 0;
        cloan=new("/secure/obj/snooper");
        cloan->eventStartSnoop(str);
        err = catch(SaveObject(SaveFile, 1));
    }
    return 1;
}

void CheckSnooped(){
    object *lusers = users();
    just_loaded = 0;
    foreach(object user in lusers){
        CheckBot(user->GetKeyName());
    }
    prevusers = lusers;
}

int SnoopClean(){
    object *voyeurs = users();
    snoopers = filter(objects(), (: base_name($1) == "/secure/obj/snooper" :) );
    if(sizeof(voyeurs)){
        foreach(object perv in voyeurs){
            if(!snooperp(perv) && !archp(perv)){
                RemoveWatcher(perv->GetKeyName(), "all");
            }
        }
    }
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

int eventDestruct(){
    if( !(master()->valid_apply(({ "SECURE" }))) )
        error("Illegal attempt to destruct snoop: "+get_stack()+" "+identify(previous_object(-1)));
    return ::eventDestruct();
}

void heart_beat(){
    count++;

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
    SaveObject(SaveFile, 1);
    return 1;
}

int RemoveWatcher(string watcher, mixed target){
    string *targs = ({});
    if(this_player() && interactive(this_player())){
        if(this_player()->GetKeyName() != watcher && !archp(this_player())) return 0;
    }
    else return 0;
    if(stringp(target)){
        if(target == "all"){
            foreach(string key, string *sap in Watchers){ 
                if(member_array(watcher,sap) != -1) targs += ({ key });
            }
        }
        else targs = ({ target });
    }
    else if(arrayp(target)) targs = target;
    else return 0;
    foreach(string subtarg in targs){
        if(!Watchers[subtarg] || !sizeof(Watchers[subtarg])) return 0;
        else if(member_array(watcher, Watchers[subtarg]) != -1) 
            Watchers[subtarg] -= ({ watcher });
        if((!Watchers[subtarg] || !sizeof(Watchers[subtarg])) &&
                member_array(subtarg, monitored) == -1) {
            foreach(object snoopbox in snoopers){
                if(snoopbox->GetSnooped() == subtarg) snoopbox->eventDestruct(); 
            }
        }
        if(!Watchers[subtarg] || !sizeof(Watchers[subtarg])) {
            snooped -= ({subtarg});
            if(Watchers[subtarg]) map_delete(Watchers, subtarg);
        }
    }
    CheckBot("adsfgrertgrsgnfxmy");
    SaveObject(SaveFile, 1);
    return 1;
}

int AddMonitor(string requestor, string target){
    if(member_array(target, monitored) == -1) monitored += ({ target });
    SaveObject(SaveFile, 1);
    CheckBot(target);
    return 1;
}

int RemoveMonitor(object requestor, string target){
    if(!archp(requestor)) return 0;
    monitored -= ({ target });
    SaveObject(SaveFile, 1);
    if(Watchers[target] && sizeof(Watchers[target])) return 1;
    foreach(object snoopbox in snoopers){
        if(snoopbox->GetSnooped() == target) snoopbox->eventDestruct(); 
    }
    if(Watchers[target] && !sizeof(Watchers[target])) map_delete(Watchers, target);
    CheckBot("asreg54eqwhtrbsf");
    SaveObject(SaveFile, 1);
    return 1;
}

int ReportLinkDeath(string str){
    if(!find_player(str)){
        foreach(object snoopbox in snoopers){
            if(snoopbox->GetSnooped() == str) snoopbox->eventDestruct(); 
        }
        map_delete(Watchers, str);
        SaveObject(SaveFile, 1);
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
        SaveObject(SaveFile, 1);
        return 1;
    }
    return 0;
}

string Report(){
    string ret = "";
    mapping TmpWatchers = ([]);
    if( !(master()->valid_apply(({ PRIV_SECURE }))) ){
        return 0;
    }
    foreach(mixed key, mixed val in Watchers){
        if(sizeof(val))
            TmpWatchers[key] = val;
    }
    ret+="Watchers: "+identify(TmpWatchers)+"\n";
    ret+="snoopers: "+identify(snoopers)+"\n";
    ret+="snooped: "+identify(snooped)+"\n";
    ret+="monitored: "+identify(monitored)+"\n";
    return ret;
}

int NotifyBot(string bot){
    mixed *arcs;
    if(base_name(previous_object()) != PLAYERS_D) return 0;
    arcs = filter(get_dir("/secure/log/adm/archive/"),
            (: !strsrch($1, $(bot)+".") :) );
    foreach(string file in arcs){
        if(!file) continue;
        rename("/secure/log/adm/archive/"+file,
                "/secure/log/adm/archive/_bot."+file);
    }
    arcs = filter(get_dir("/secure/log/adm/"),
            (: !strsrch($1, $(bot)+".") :) );
    foreach(string file in arcs){
        if(!file) continue;
        rename("/secure/log/adm/"+file,
                "/secure/log/adm/_bot."+file);
    }
    return 1;
}
