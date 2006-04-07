// This file written completely by Tim Johnson (Tim@TimMUD)
#include <network.h>

static void create(){ 
    SetNoClean(1);
    //save::create();
    sockets = ([]);
    connected_muds = ([]);
    if(!mudinfo) mudinfo = ([]);
    if(!mudinfo_updates) mudinfo_updates = ([]);
    if(!mudinfo_update_counter) mudinfo_update_counter = 1;
    if(!listening) listening = ([]);
    if(!channels) channels = ([]);
    if(!channel_updates) channel_updates = ([]);
    if(!channel_update_counter) channel_update_counter = 1;
    if(!router_name) router_name = "*yatmim";
    if(!router_list) router_list = ({ ({"*yatmim",  "149.152.218.102 23"}) });
    log_file("server", "Created when uptime = " + uptime() + "\n");
    Debug("server got created",DEB_OTHER);
    call_out("setup", 5);
}

static void setup(){
    Debug("setup got called");
    if ((router_socket = socket_create(MUD, "read_callback", "close_callback")) < 0){
	log_file("server", "setup: Failed to create socket.\n");
	return;
    }
    if (socket_bind(router_socket, MYSERVER) < 0) {
	socket_close(router_socket);
	log_file("server", "setup: Failed to bind socket to port.\n");
	return;
    }
    if (socket_listen(router_socket, "listen_callback") < 0) {
	socket_close(router_socket);
	log_file("server", "setup: Failed to listen to socket.\n");
    }
    Debug("setup ended");
}

void remove(){
    string mudname;
    log_file("server", "Being removed by: "+identify(previous_object())+"\n");
    log_file("server", "sockets:"+identify(sockets)+"\n");

    log_file("server", "Starting to destruct at uptime = " + uptime() + "\n");
    mudinfo_update_counter++; // just increment once... all the connected muds
    // getting simultaneously disconnected is okay to count as one change :)
    foreach(mudname in keys(connected_muds)){
	mudinfo_updates[mudname];
	mudinfo[mudname]["disconnect_time"]=time();
	map_delete(connected_muds,mudname); // unneeded: connected_muds isn't saved anyway
    }
    log_file("server", "Successfully destructed when uptime = " + uptime() + "\n");
    destruct(this_object());
}
int query_prevent_shadow(object ob){ return 1; }
