#include <lib.h>
#include <config.h>
#include <rooms.h>
#include <daemons.h>
#include <commands.h>

inherit LIB_DAEMON;
mapping Reloadees = ([]);
static int *reload_handles = ({});
static int warm_boot_in_progress = 0;
string savefile = "/secure/save/reload";
static string *exceptions = ({ RELOAD_D, RSOCKET_D });

varargs void validate(){
    if((!(int)master()->valid_apply(({ "SECURE", "ASSIST" })))){
        string offender = identify(previous_object(-1));
        debug("RELOAD_D SECURITY VIOLATION: "+offender+" ",get_stack(),"red");
        log_file("security", "\n"+timestamp()+" RELOAD_D breach: "+offender+" "+get_stack());
        error("RELOAD_D SECURITY VIOLATION: "+offender+" "+get_stack());
    }
}

static void create() {
    daemon::create();
    if(!file_exists(savefile+__SAVE_EXTENSION__)) save_object(savefile);
    else restore_object(savefile);
    set_heart_beat(1);
}

int ReloadBaseSystem(){
    string *tmp = get_dir("/secure/sefun/");
    string *sefun_files = ({});
    foreach(string file in tmp){
        if(!strsrch(file,"sefun.")) continue;
        sefun_files += ({ "/secure/sefun/"+file });
    }
    foreach(string file in sefun_files){
        update(file);
    }
    update(SEFUN);
    update(MASTER_D);
    return 1;
}

mixed ReloadPlayer(mixed who){
    mixed mx;
    string name;
    object tmp_bod, new_bod;

    validate();

    if(stringp(who)) who = find_player(who);
    if(!who) return 0;

    name = who->GetKeyName();
    //tc("who: "+identify(who));
    //tc("name: "+identify(name));
    who->save_player(name);
    mx = reload(load_object(LIB_CREATOR), 0, 0);
    if(mx) mx = reload(load_object(LIB_PLAYER), 0, 0);

    if(!mx) error("OHSHI-");

    tmp_bod = new(LIB_PLAYER_STUB);
    tmp_bod->SetKeyName(name);

    who->eventMove(ROOM_POD);

    mx = exec(tmp_bod, who);

    //tc("mx: "+identify(mx),"blue");
    //tc("who: "+identify(who),"blue");
    //tc("tmp_bod: "+identify(tmp_bod),"blue");
    who->eventMove(ROOM_FURNACE);
    who->eventDestruct();

    new_bod = (object)master()->player_object(name, tmp_bod); 

    if( !new_bod ) return 0;

    //tc("new_bod: "+identify(new_bod));

    mx = exec(new_bod, tmp_bod);

    if(!mx){
        //tc("exec faild!");
        return 0;
    }

    destruct(tmp_bod);
    new_bod->Setup();
    SNOOP_D->CheckBot(name);
    return 1;
}

varargs int eventUpdate(mixed what){
    if(what->GetVirtual()){
        return what->eventDestruct();
    }
    return update(what);
}

varargs int eventReload(mixed what, int when, int nodelay){
    if(!what) return 0;
    if(what->GetVirtual()) return what->eventDestruct();
    if(nodelay) return reload(what, 0, 1);
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
    if(warm_boot_in_progress){
        Reloadees = ([]);
        foreach(mixed element in reload_handles){
            if(!intp(element)){
                reload_handles -= ({ element });
                continue;
            }
            if(find_call_out(element) == -1){
                reload_handles -= ({ element });
            }
        }
        if(!sizeof(reload_handles)){
            warm_boot_in_progress = 0;
            this_object()->ReloadUsers();
            shout("Warm boot complete.");
            eventSave();
        }
    }
    foreach(mixed key, mixed val in Reloadees){
        if(time() >= val){
            map_delete(Reloadees,key);
            key = find_object(key);
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

int ReloadDir(string dir, int passes){
    //object *lib_obs = objects( (: grepp(base_name($1),$(dir)) :) );
    object *lib_obs = objects( (: !strsrch(base_name($1),$(dir)) :) );
    int err;
    validate();
    //lib_obs = filter( lib_obs, (: !strsrch(base_name($1),$(dir)) :) );
    if(!passes) passes = 2;
    while(passes){
        //tc("Reloading "+dir);
        foreach(object ob in lib_obs){
            if(ob != this_object() && 
              member_array(base_name(ob), exceptions) == -1){
                if(ob && inherits(LIB_ROOM,ob) && sizeof(livings(ob))){
                    reload_handles += ({ call_out("eventReload", 5,ob, 0, 1) });
                    //err = catch(reload(ob, 0, 1));
                }
                else reload_handles += ({ call_out("eventUpdate", 5, base_name(ob)) });
                //else err = catch(update(base_name(ob)));
            }
        }
        passes--;
    }
    return 1;
}

int ReloadUsers(){
    int err, mx, ret = 1;
    validate();
    mx = reload(load_object(LIB_CREATOR), 1, 1);
    if(!mx) error("OHFUC-");

    foreach(object player in users()){
        err = catch(RELOAD_D->ReloadPlayer(player));
        if(err) ret = 0;
    }
    return ret;
}

int ReloadMud(){
    string *dir2 = ({ "/lib/", "/secure/","/daemon/" });
    string *dir1 = ({ "/cmds/", "/verbs/","/estates/", "/obj/", "/open/", "/shadows/", "/spells/", "/std/" });
    validate();
    shout("Warm boot initiated!");
    flush_messages();
    ReloadBaseSystem();
    foreach(string dir in (dir2 + dir1)){
        ReloadDir(dir, ((member_array(dir, dir1) != -1) ? 1 : 2));
    }
    ReloadDir("/domains/", 2);
    ReloadDir("/realms/", 2);
    warm_boot_in_progress = 1;
    //ReloadUsers();
    //shout("Warm boot complete.");
    return 1;
}

int WarmBoot(){
    validate();
    call_out("ReloadMud", 0);
    return 1;
}
