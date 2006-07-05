// I3 server.
// This file written completely by Tim Johnson (Tim@TimMUD)
// Started by Tim on May 7, 2003.
// http://cie.imaginary.com/protocols/intermud3.html#errors
#include <lib.h>
#include <socket.h>
#include <socket_err.h>
#define DEB_IN  1	// trr-Incoming
#define DEB_OUT 2	// trr-Outgoing
#define DEB_INVALID 3	// trr-Invalid
#define DEB_OTHER 0	// trr-Other
#define MYSERVER 9000	// Port to accept connections on.
#define DEBUGGER_GUY "cratylus"	// Name of who to display trrging info to.
#undef DEBUGGER_GUY
#define DEBUGGER_GUY "guest"
#define MAXIMUM_RETRIES 5
// SEND_WHOLE_MUDLIST makes it act like the official I3 server instead of like the I3 specs
#define SEND_WHOLE_MUDLIST
// SEND_WHOLE_CHANLIST makes it act like the official I3 server instead of like the I3 specs
#define SEND_WHOLE_CHANLIST
inherit LIB_CLEAN;

// Unsaved variables...
static private int router_socket;
// socket that the router is using
static private mapping sockets;
// physically connected sockets and their info
static private mapping connected_muds;
// muds that have successfully done a startup
// (key=mudname, value=fd)
//static private
mapping listening;
// list of muds listening to each channel
// (key=chan name, value=mud array)

// Saved variables...
string router_name; // Name of the router.
string *router_list; // Ordered list of routers to use.
mapping mudinfo; // Info about all the muds which the router knows about.
mapping channels; // Info about all the channels the router handles.
mapping channel_updates; // Tells when a channel was last changed.
int channel_update_counter; // Counter for the most recent change.
// Why is this not a part of the channels mapping?
// Because I need to remember that some channels got deleted.
mapping mudinfo_updates; // Like channel_updates except for muds.
int mudinfo_update_counter; // Similar to channel_update_counter

// Prototypes
void write_data(int fd, mixed data);
void close_connection(int fd);
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
mapping query_mudinfo(){ return copy(mudinfo); }
mapping query_mud(string str){ return copy(mudinfo[str]); }
void get_info() {
    write_file ("/tmp/info.txt",
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
}
void clear(){ string mudname; foreach(mudname in keys(mudinfo)) remove_mud(mudname); 
}

mapping GetConnectedMuds(){
    return copy(connected_muds);
}

string *AddBannedMud(string str){
    if( !((int)master()->valid_apply(({ "SECURE" }))) )
	error("Illegal attempt to access admintool: "+get_stack()+" "+identify(previous_object(-1)));

    banned_muds += ({ str });
    return banned_muds;
}

string *RemoveBannedMud(string str){
    if( !((int)master()->valid_apply(({ "SECURE" }))) )
	error("Illegal attempt to access admintool: "+get_stack()+" "+identify(previous_object(-1)));

    banned_muds -= ({ str });
    return banned_muds;
}

void clear_discs(){ 
    string mudname; 
    foreach(mudname in keys(mudinfo)) {
	if(query_mud(mudname)["disconnect_time"] &&
	  time() - query_mud(mudname)["disconnect_time"] > 60 &&
	  time() - query_mud(mudname)["disconnect_time"] < 80){
	    //trr("Removing mud: "+identify(mudname),"red");
	    //remove_mud(mudname);
	    trr("Broadcasting updated mudlist.","white");
	    broadcast_mudlist(mudname);

	}
    }
}

