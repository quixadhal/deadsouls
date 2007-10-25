#include <network.h>
#include <daemons.h>
#include <save.h>

int heart_count = 0;
static int reset_me = 0;

string router_port;

static void create(){ 
    object rsock = find_object(RSOCKET_D);
    if(!rsock){
        server_log("Strange. No rsocket.");
        rsock = load_object(RSOCKET_D);
        rsock->irn_clear();
    }
    if(!rsock) return;
    SetNoClean(1);
    connected_muds = ([]);
    unguarded( (: restore_object, SAVE_ROUTER, 1 :) );
    if(!mudinfo) mudinfo = ([]);
    if(!mudinfo_updates) mudinfo_updates = ([]);
    if(!mudinfo_update_counter) mudinfo_update_counter = 1;
    if(!listening) listening = ([]);
    if(!channels) channels = ([]);
    if(!channel_updates) channel_updates = ([]);
    if(!channel_update_counter) channel_update_counter = 1;
    if(!router_name) router_name = "*yatmim";
    if(!router_port) router_port = "9000";
    if(!router_ip) router_ip = "149.152.218.102";
    if(mud_name() == "Frontiers")
        router_list = ({ ({"*yatmim", "149.152.218.102 23"}) });
    server_log("Created when mud uptime = " + time_elapsed(uptime()) + "\n");
    server_log("rsocket uptime = " + time_elapsed(time()-RSOCKET_D->GetInceptDate()) + "\n");
    call_out("setup", 1);
    call_out("LocalHostedChans", 15);
    this_object()->purge_crud();
    set_heart_beat(10);
    reset_me = 0;
    if(file_exists(ROUTER_BLACKLIST)){
        blacklisted_muds += explode(read_file(ROUTER_BLACKLIST),"\n");
        blacklisted_muds = singular_array(blacklisted_muds);
    }
    this_object()->irn_checkstat();
    this_object()->check_blacklist();
}

int SetReset(){
    if(base_name(previous_object()) != "/domains/campus/room/tricky") return 0;
    //trr("objects: "+identify(previous_object(-1)),"red");
    reset_me = 1;
}

void heart_beat(){
    heart_count++;
    this_object()->irn_checkstat();
    //if(reset_me) RELOAD_D->eventReload(this_object(), 2);
    if(!(heart_count % 60)) {
        //trr("CLOSING OLD/DISCONNECTED/PARADOXED SOCKETS","white");
        //this_object()->irn_checkstat();
        this_object()->check_discs();
        save_object(SAVE_ROUTER);
    }
    if(heart_count > 3600){
        heart_count = 0;
        this_object()->clear_discs();
        this_object()->check_blacklist();
    }
}

static void setup(){
    if( file_size( SAVE_ROUTER __SAVE_EXTENSION__ ) > 0 )
        unguarded( (: restore_object, SAVE_ROUTER, 1 :) );
    //irn_setup();
}

int query_prevent_shadow(object ob){ return true(ob); }
