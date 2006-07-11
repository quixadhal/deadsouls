// This file written completely by Tim Johnson (Tim@TimMUD)
#include <network.h>
#include <save.h>

static void create(){ 
    SetNoClean(1);
    sockets = ([]);
    connected_muds = ([]);
    if(!mudinfo) mudinfo = ([]);
    if(!mudinfo_updates) mudinfo_updates = ([]);
    if(!mudinfo_update_counter) mudinfo_update_counter = 1;
    if(!listening) listening = ([]);
    if(!channels) channels = ([]);
    if(!channel_updates) channel_updates = ([]);
    if(!channel_update_counter) channel_update_counter = 1;
    if(!router_name) router_name = "*crossing";
    router_list = ({ ({"*crossing", "192.168.0.5 9000"}) });
    log_file("server", "Created when uptime = " + uptime() + "\n");
    trr("server got created");
    log_file("i3router",timestamp()+" router object created.");
    call_out("setup", 5);
    call_out("LocalHostedChans", 15);
    set_heart_beat(10);
}

void heart_beat(){
    trr("BING!","white");
    this_object()->clear_discs();
}


static void setup(){
    trr("setup got called");
    if( file_size( SAVE_ROUTER __SAVE_EXTENSION__ ) > 0 )
	unguarded( (: restore_object, SAVE_ROUTER, 1 :) );
    if ((router_socket = socket_create(MUD, "read_callback", "close_callback")) < 0){
	log_file("server", "setup: Failed to create socket.\n");
	trr("setup: Failed to create socket.\n");
	return;
    }
    if (socket_bind(router_socket, MYSERVER) < 0) {
	socket_close(router_socket);
	log_file("server", "setup: Failed to bind socket to port.\n");
	trr("setup: Failed to bind socket to port.\n");
	return;
    }
    if (socket_listen(router_socket, "listen_callback") < 0) {
	socket_close(router_socket);
	log_file("server", "setup: Failed to listen to socket.\n");
	trr("setup: Failed to listen to socket.\n");
    }
    trr("setup ended");
}

void remove(){
    string mudname;
    log_file("server", "Being removed by: "+identify(previous_object())+"\n");
    trr("Being removed by: "+identify(previous_object())+"\n","red");
    log_file("server", "sockets:"+identify(sockets)+"\n");
    trr("sockets:"+identify(sockets)+"\n");
    log_file("server", "Starting to destruct at uptime = " + uptime() + "\n");
    trr("Starting to destruct at uptime = " + uptime() + "\n");
    mudinfo_update_counter++; // just increment once... all the connected muds
    // getting simultaneously disconnected is okay to count as one change :)
    foreach(mudname in keys(connected_muds)){
	mudinfo_updates[mudname];
	mudinfo[mudname]["disconnect_time"]=time();
	map_delete(connected_muds,mudname); // unneeded: connected_muds isn't saved anyway
    }
    log_file("server", "Successfully destructed when uptime = " + uptime() + "\n");
    trr("Successfully destructed when uptime = " + uptime() + "\n");
    destruct(this_object());
}
int query_prevent_shadow(object ob){ return 1; }
