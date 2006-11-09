// This file written completely by Tim Johnson (Tim@TimMUD)
#include <network.h>
#include <daemons.h>
#include <save.h>
int heart_count = 0;

string router_port;

static void create(){ 
    SetNoClean(1);
    sockets = ([]);
    connected_muds = ([]);
    unguarded( (: restore_object, SAVE_ROUTER, 1 :) );
    if(!mudinfo) mudinfo = ([]);
    if(!mudinfo_updates) mudinfo_updates = ([]);
    if(!mudinfo_update_counter) mudinfo_update_counter = 1;
    if(!listening) listening = ([]);
    if(!channels) channels = ([]);
    if(!channel_updates) channel_updates = ([]);
    if(!channel_update_counter) channel_update_counter = 1;
    if(!router_name) router_name = "*Dead Souls";
    if(!router_port) router_port = "9000";
    if(!router_ip) router_ip = "192.168.0.4";
    //if(!sizeof(router_list))
    router_list = ({ ({"*Dead Souls", "192.168.0.4 9000"}) });
    log_file("router/server_log", "Created when uptime = " + uptime() + "\n");
    trr("server got created");
    log_file("router/server_log",timestamp()+" router object created.\n");
    call_out("setup", 5);
    call_out("LocalHostedChans", 15);
    set_heart_beat(10);
}

void heart_beat(){
    heart_count++;
    if(heart_count % 5) trr("BING!","white");
    if(heart_count % 20) this_object()->clear_discs();
    if(heart_count > 300) {
	heart_count = 0;
	this_object()->clean_socks();
	save_object(SAVE_ROUTER);
    }
}

static void setup(){
    trr("setup got called");
    RELOAD_D->eventReload(this_object(),90000+random(10000));
    log_file("router/server_log",timestamp()+" setup has been called.\n");
    if( file_size( SAVE_ROUTER __SAVE_EXTENSION__ ) > 0 )
	unguarded( (: restore_object, SAVE_ROUTER, 1 :) );
    if ((router_socket = socket_create(MUD, "read_callback", "close_callback")) < 0){
	log_file("router/server_log", timestamp()+"setup: Failed to create socket.\n");
	trr("setup: Failed to create socket.\n");
	return;
    }
    if (socket_bind(router_socket, atoi(router_port)) < 0) {
	socket_close(router_socket);
	log_file("router/server_log", timestamp()+"setup: Failed to bind socket to port.\n");
	trr("setup: Failed to bind socket to port.\n");
	return;
    }
    if (socket_listen(router_socket, "listen_callback") < 0) {
	socket_close(router_socket);
	log_file("router/server_log", timestamp()+"setup: Failed to listen to socket.\n");
	trr("setup: Failed to listen to socket.\n");
    }
    trr("setup ended");
    log_file("router/server_log",timestamp()+" setup successful.\n");
    log_file("router/server_log",timestamp()+" open sockets: "+identify(this_object()->open_socks())+"\n");
    log_file("router/server_log",timestamp()+" cleaning sockets...\n");
    this_object()->clean_socks();
    log_file("router/server_log",timestamp()+" open sockets: "+identify(this_object()->open_socks())+"\n");
}

void remove(){
    string mudname;
    log_file("router/server_log", timestamp()+" Being removed by: "+identify(previous_object())+"\n");
    trr("Being removed by: "+identify(previous_object())+"\n","red");
    log_file("router/server_log", "sockets:"+identify(sockets)+"\n");
    trr("sockets:"+identify(sockets)+"\n");
    log_file("router/server_log", "Starting to destruct at uptime = " + uptime() + "\n");
    trr("Starting to destruct at uptime = " + uptime() + "\n");
    mudinfo_update_counter++; // just increment once... all the connected muds
    // getting simultaneously disconnected is okay to count as one change :)
    foreach(mudname in keys(connected_muds)){
	mudinfo_updates[mudname]++;
	mudinfo[mudname]["disconnect_time"]=time();
	map_delete(connected_muds,mudname); // unneeded: connected_muds isn't saved anyway
    }
    log_file("router/server_log", "Successfully destructed when uptime = " + uptime() + "\n");
    trr("Successfully destructed when uptime = " + uptime() + "\n");
    destruct(this_object());
}
int query_prevent_shadow(object ob){ return true(ob); }
