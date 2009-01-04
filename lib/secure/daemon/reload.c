#include <lib.h>
#include <config.h>
#include <rooms.h>
#include <daemons.h>
#include <commands.h>

inherit LIB_DAEMON;
mapping Reloadees = ([]);
static int *reload_handles = ({});
static int stage2, stilldirty, roomscleaned, warm_boot_in_progress = 0;
string savefile = "/secure/save/reload";
static string *exceptions = ({ RELOAD_D, RSOCKET_D });
object *grooms = ({});

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
    if(!file_exists(savefile+__SAVE_EXTENSION__)) 
        unguarded( (: save_object(savefile) :) );
    else unguarded( (: restore_object(savefile) :) );
    set_heart_beat(1);
}

varargs void eventDestructEmptyRooms(object room, int last){
    validate();
    if(last && !stilldirty) roomscleaned = 1;
    else roomscleaned = 0;
    if(room){
        if(objectp(room) && !sizeof(get_livings(room,1))){
            object *npcs = get_livings(room,2);
            if(sizeof(npcs)) npcs->eventDestruct();
            catch( room->eventDestruct() );
        }
    }
}

void eventResetEmptyRooms(){
    int i, j, k;
    object *rooms = objects((: inherits(LIB_ROOM, $1) :))[0..64000];
    object *rooms2;
    validate();
    grooms = rooms;
    stilldirty = 1;
    for(i = sizeof(rooms)-1; j < i;j++){
        int last = (j > (i - 10) );
        reset_eval_cost();
        if(!rooms[j]) continue;
        if(j < 10){
            eventDestructEmptyRooms(rooms[j], last);
        }
        else {
            k = call_out((: eventDestructEmptyRooms :), j/100, rooms[j], last);
        }
        if(k) reload_handles += ({ k });
    }
    rooms2 =  objects((: inherits(LIB_ROOM, $1) :)) - rooms;
    grooms = rooms2;
    if(sizeof(rooms2)){
        call_out( "eventResetEmptyRooms", ((j/100)+1));
    }
    else stilldirty = 0;
}

int ReloadBaseSystem(){
    string *tmp = get_dir("/secure/sefun/");
    string *sefun_files = ({});
    catch( update(MASTER_D) );
    foreach(string file in tmp){
        if(!strsrch(file,"sefun.")) continue;
        sefun_files += ({ "/secure/sefun/"+file });
    }
    foreach(string file in sefun_files){
        catch(update(file));
    }
    RELOAD_D->eventReload(load_object(SEFUN), 1);
    catch( update(MASTER_D) );
    catch( reload(load_object(LIB_CREATOR), 1, 1) );
    catch( reload(load_object(LIB_SENTIENT), 1, 1) );
    catch( reload(load_object(LIB_ROOM), 1, 1) );
    catch( reload(load_object(LIB_ARMOR), 1, 1) );
    catch( reload(load_object(LIB_STORAGE), 1, 1) );
    catch( reload(load_object(LIB_WORN_STORAGE), 1, 1) );
    return 1;
}

varargs mixed ReloadPlayer(mixed who, int deep){
    mixed mx;
    string name;
    object tmp_bod, new_bod;
    int charmode;

    validate();

    if(stringp(who)) who = find_player(who);
    if(!who) return 0;

    name = who->GetKeyName();
    charmode = who->GetCharmode();
    who->CancelCharmode();
    who->save_player(name);
    mx = reload(load_object(LIB_CREATOR), deep, 0);
    if(mx) mx = reload(load_object(LIB_PLAYER), deep, 0);

    if(!mx) error("OHSHI-");

    tmp_bod = new(LIB_PLAYER_STUB);
    tmp_bod->SetKeyName(name);

    who->eventMove(ROOM_POD);

    mx = exec(tmp_bod, who);

    who->eventMove(ROOM_FURNACE);
    who->eventDestruct();

    new_bod = (object)master()->player_object(name, tmp_bod); 

    if( !new_bod ) return 0;

    mx = exec(new_bod, tmp_bod);

    if(!mx){
        return 0;
    }

    destruct(tmp_bod);
    new_bod->Setup();
    //new_bod->SetCharmode(charmode);
    SNOOP_D->CheckBot(name);
    return 1;
}

varargs int eventUpdate(mixed what){
    mixed tmpwhat = what;
    int err;
    if(stringp(what)){
        what = find_object(tmpwhat);
    }
    if(!objectp(what)){
        err = catch(what = load_object(tmpwhat));
    }
    if(err || !objectp(what)){
        return 0;
    }
    if(what->GetVirtual()){
        return what->eventDestruct();
    }
    if(!inherits(LIB_ROOM, what)) return update(base_name(what));
    else {
        tmpwhat = base_name(what);
        what->eventDestruct();
        return update(tmpwhat);
    }
}

varargs int eventReload(mixed what, int when, int nodelay){
    if(!what) return 0;
    if(what->GetVirtual()){
        object *npcs = get_livings(what,2);
        if(sizeof(npcs)) npcs->eventDestruct();
        return what->eventDestruct();
    }
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
    if(warm_boot_in_progress && !stage2){
        this_object()->ReloadMud2();
    }
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

        if(!sizeof(grooms)){
            stilldirty = 0;
            if(!sizeof(reload_handles)){
                roomscleaned = 1;
            }
        }

    }
    if(warm_boot_in_progress && stage2){
        if(!sizeof(reload_handles)){
            shout("Reloading users...");
            flush_messages();
            this_object()->ReloadUsers();
            shout("Warm boot complete.");
            warm_boot_in_progress = 0;
            stage2 = 0;
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
    object *lib_obs = objects( (: !strsrch(base_name($1),$(dir)) :) );
    int err;
    validate();
    lib_obs = filter(lib_obs, (: !clonep($1) || 
                (clonep($1) && environment($1)) :));
    debug("Size of ReloadDir("+dir+", "+passes+"): "+sizeof(lib_obs));
    if(!passes) passes = 3;
    while(passes){
        reset_eval_cost();
        foreach(object ob in lib_obs){
            reset_eval_cost();
            if(ob != this_object() && 
                    member_array(base_name(ob), exceptions) == -1){
                if(ob && inherits(LIB_ROOM,ob) && sizeof(livings(ob,1))){
                    reload_handles += ({ call_out((: eventReload :), 5,ob, 0, 1) });
                }
                else reload_handles += ({ call_out((: eventUpdate :), 5, base_name(ob)) });
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
        int invis = player->GetInvis();
        string pstr = base_name(player);
        reset_eval_cost();
        player->SetInvis(0);
        err = catch(RELOAD_D->ReloadPlayer(player));
        if(err){
            debug("problem reloading "+pstr,"red");
            ret = 0;
        }
        else find_object(pstr)->SetInvis(invis);
    }
    return ret;
}

int ReloadMud1(){
    validate();
    shout("Warm boot initiated!");
    warm_boot_in_progress = 1;
    roomscleaned = 0;
    shout("Rooms resetting..."); 
    flush_messages();
    eventResetEmptyRooms();
    return 1;
}

int ReloadMud2(){
    string *dir2;
    string *dir1;
    validate();
    if(!roomscleaned || stilldirty) return 0;
    dir2 = ({ "/lib/", "/secure/","/daemon/" });
    dir1 = ({ "/cmds/", "/verbs/","/estates/", "/obj/", "/open/", 
            "/shadows/", "/std/", "/powers/" });

    stage2 = 1;
    ReloadBaseSystem();
    shout("Initializing base system...");
    flush_messages();
    foreach(string dir in (dir2 + dir1)){
        shout("Initializing: "+dir);
        flush_messages();
        reset_eval_cost();
        ReloadDir(dir, ((member_array(dir, dir1) != -1) ? 1 : 2));
    }
    shout("Reloading domains. This can take a few minutes...");
    flush_messages();
    ReloadDir("/domains/", 3);
    shout("Reloading realms...");
    flush_messages();
    ReloadDir("/realms/", 3);
    return 1;
}

int WarmBoot(){
    validate();
    call_out( (: ReloadMud1 :), 0);
    return 1;
}

int GetWarmBootInProgress(){
    return warm_boot_in_progress;
}
