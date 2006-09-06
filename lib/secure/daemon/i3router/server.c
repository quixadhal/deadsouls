// I3 server.
// This file written mostly by Tim Johnson (Tim@TimMUD)
// Started by Tim on May 7, 2003.
// http://cie.imaginary.com/protocols/intermud3.html#errors
#include <lib.h>
#include <commands.h>
#include <socket.h>
#include <socket_err.h>
#define DEB_IN  1	// trr-Incoming
#define DEB_OUT 2	// trr-Outgoing
#define DEB_INVALID 3	// trr-Invalid
#define DEB_OTHER 0	// trr-Other
#define DEBUGGER_GUY "cratylus"	// Name of who to display trrging info to.
#undef DEBUGGER_GUY
#define DEBUGGER_GUY "MXLPLX"
#define MAXIMUM_RETRIES 5
// SEND_WHOLE_MUDLIST makes it act like the official I3 server instead of like the I3 specs
#define SEND_WHOLE_MUDLIST
// SEND_WHOLE_CHANLIST makes it act like the official I3 server instead of like the I3 specs
#define SEND_WHOLE_CHANLIST
inherit LIB_CLEAN;

static void validate(){
    if( previous_object() != CMD_ROUTER &&
      !((int)master()->valid_apply(({ "ASSIST" }))) )
	error("Illegal attempt to access router daemon: "+get_stack()+" "+identify(previous_object(-1)));
}

// Unsaved variables...
static private int router_socket;
// socket that the router is using
static private mapping sockets;
// physically connected sockets and their info
static private mapping connected_muds;
// muds that have successfully done a startup
// (key=mudname, value=fd)
//static private
static private mapping listening;
// list of muds listening to each channel
// (key=chan name, value=mud array)

// Saved variables...
string router_name; // Name of the router.
string router_ip;
string *router_list = ({}); // Ordered list of routers to use.
mapping mudinfo = ([]); // Info about all the muds which the router knows about.
mapping channels; // Info about all the channels the router handles.
mapping channel_updates; // Tells when a channel was last changed.
int channel_update_counter; // Counter for the most recent change.
// Why is this not a part of the channels mapping?
// Because I need to remember that some channels got deleted.
mapping mudinfo_updates; // Like channel_updates except for muds.
int mudinfo_update_counter; // Similar to channel_update_counter

// Prototypes
void write_data(int fd, mixed data);
static mapping muds_on_this_fd(int fd);
static mapping muds_not_on_this_fd(int fd);
static void broadcast_data(mapping targets, mixed data);
// Ones with their own files...
static void broadcast_chanlist(string channame);
static void broadcast_mudlist(string mudname);
static varargs void Debug(string str, int level);
static void process_channel(int fd, mixed *info);
static void process_startup_req(int protocol, mixed info, int fd);
static void read_callback(int fd, mixed info);
static void remove_mud(string mudname);
static void send_chanlist_reply(string mudname, int old_chanid);
static void send_mudlist(string mudname);
static void send_mudlist_updates(string updating_mudname, int old_mudlist_id);
static void send_startup_reply(string mudname);
static void send_error(string mud, string user, string errcode, string errmsg, mixed *info);
// core_stuff.h...
static void create();
static void setup();
void remove();
// funcs.h...
static mapping muds_on_this_fd(int fd);
int value_equals(string a,int b, int c);
static mapping muds_not_on_this_fd(int fd);
int value_not_equals(string a,int b, int c);
// socket_stuff.h
static void close_callback(int fd);
static void listen_callback(int fd);
static void write_data_retry(int fd, mixed data, int counter);
static void close_connection(int fd);
static void write_data(int fd, mixed data);
static void broadcast_data(mapping targets, mixed data);

// Code for all the stuff in the prototypes...
#include "./broadcast_chanlist.h"
#include "./broadcast_mudlist.h"
#include "./debug.h"
#include "./process_channel.h"
#include "./process_startup_req.h"
#include "./read_callback.h"
#include "./remove_mud.h"
#include "./send_chanlist_reply.h"
#include "./send_mudlist_updates.h"
#include "./send_startup_reply.h"
#include "./send_error.h"

#include "./core_stuff.h"
#include "./funcs.h"
#include "./socket_stuff.h"
#include "./hosted_channels.h"

// trrging stuff...
mapping query_mudinfo(){ validate(); return copy(mudinfo); }
mapping query_mud(string str){ validate(); return copy(mudinfo[str]); }
mapping query_connected_muds(){ validate(); return copy(connected_muds); }
mapping query_socks(){ validate(); return copy(sockets); }

mapping query_connected_fds(){
    mapping RetMap = ([]);
    validate();
    foreach(mixed key, mixed val in connected_muds){
	RetMap[val] = key;
    }
    return copy(RetMap);
}

int *open_socks(){
    int *ret = ({});
    validate();
    foreach(mixed element in socket_status()){
	if(intp(element[0]) && element[0] != -1 && !grepp(element[3],"*") &&
	  last_string_element(element[3],".") == router_port) {
	    ret += ({ element[0] });
	}
    }
    return ret;
}

int clean_socks(){
    int *socky = open_socks();
    int *conn_socky = keys(query_connected_fds());
    foreach(int fd in socky){
	if(member_array(fd, conn_socky) == -1) close_connection(fd);
    }
    return sizeof(open_socks());
}

void get_info() {
    mixed *socky = open_socks();
    string socks = implode(socky, " ");
    int socknum = sizeof(socky);
    validate();

    socks += "\nTotal number of connected muds: "+socknum+"\n";
    write_file ("/secure/tmp/info.txt",
      "router_name: "+router_name+
      "\nrouter_list"+identify(router_list)+
      "\nchannel_update_counter: "+ channel_update_counter+
      "\nchannels:"+identify(channels)+
      "\nchannel_updates:"+identify(channel_updates)+
      "\nlistening:"+identify(listening)+
      "\nmudinfo:"+identify(mudinfo)+
      "\nmudinfo_update_counter: "+ mudinfo_update_counter+
      "\nmudinfo_updates:"+identify(mudinfo_updates)+
      "\nconnected:"+identify(connected_muds)+"\n");
    write("router_name: "+router_name+
      "\nrouter_list"+identify(router_list)+
      "\nchannel_update_counter: "+ channel_update_counter+
      ((sizeof(channels)) ? "\nchannels:"+implode(keys(channels),", ") : "")+
      "\nmudinfo_update_counter: "+ mudinfo_update_counter+
      "\nsockets: "+socks);
}
void clear(){ 
    string mudname; 
    validate();
    log_file("router/server_log",timestamp()+" Clearing all mud data.\n"); 
    foreach(mudname in keys(mudinfo)) remove_mud(mudname); 
}

string GetRouterName(){
    validate();
    return router_name;
}

string SetRouterName(string str){
    validate();
    //tc("router_name: "+router_name);
    router_name = str;
    //tc("router_name: "+router_name);
    log_file("router/server_log",timestamp()+" setting router name to: "+str+"\n"); 
    save_object(SAVE_ROUTER);
    return router_name;
}

string GetRouterIP(){
    validate();
    return router_ip;
}

string SetRouterIP(string str){
    validate();
    router_ip = str;
    log_file("router/server_log",timestamp()+" setting router IP to: "+str+"\n");
    save_object(SAVE_ROUTER);
    return router_ip;
}

string GetRouterPort(){
    validate();
    return router_port;
}

string SetRouterPort(string str){
    validate();
    router_port = str;
    log_file("router/server_log",timestamp()+" setting router port to: "+str+"\n");
    save_object(SAVE_ROUTER);
    return router_port;
}

string *GetRouterList(){
    validate();
    return router_list;
}

varargs string *SetRouterList(string *str){
    string tmp;
    validate();
    return router_list;
    if(!strsrch(router_name,"*")) tmp = router_name;
    else tmp = "*"+router_name;
    if(!str || !sizeof(str)){
	router_list = ({ ({ tmp, router_ip+" "+router_port }) });
	save_object(SAVE_ROUTER);
	return router_list;
    }
    router_list = ({ str });
    log_file("router/server_log",timestamp()+" setting router list to: "+identify(router_list)+"\n");
    save_object(SAVE_ROUTER);
    return router_list;
}


mapping GetConnectedMuds(){
    validate();
    return copy(connected_muds);
}

string *GetBannedMuds(){
    validate();
    return banned_muds;
}

string *AddBannedMud(string str){
    validate();
    banned_muds += ({ str });
    log_file("router/server_log",timestamp()+" "+str+" has been BANNED\n");
    save_object(SAVE_ROUTER);
    return banned_muds;
}

string *RemoveBannedMud(string str){
    validate();
    banned_muds -= ({ str });
    log_file("router/server_log",timestamp()+" "+str+" has been unbanned.\n");
    save_object(SAVE_ROUTER);
    return banned_muds;
}

void clear_discs(){ 
    string mudname; 
    validate();
    foreach(mudname in keys(mudinfo)) {
	if(query_mud(mudname)["disconnect_time"] &&
	  time() - query_mud(mudname)["disconnect_time"] > 60){
	    //&& time() - query_mud(mudname)["disconnect_time"] < 80){
	    trr("Removing disconnected mud: "+identify(mudname),"red");
	    remove_mud(mudname);
	    trr("Broadcasting updated mudlist.","white");
	    broadcast_mudlist(mudname);

	}
    }
}

int eventDestruct(){
    save_object(SAVE_ROUTER);
    clean::eventDestruct();
}
