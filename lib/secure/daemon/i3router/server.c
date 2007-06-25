// I3 server.
// This file written mostly by Tim Johnson (Tim@TimMUD)
// Started by Tim on May 7, 2003.
// http://cie.imaginary.com/protocols/intermud3.html#errors
#include <lib.h>
#include <commands.h>
#include <daemons.h>
#include <socket.h>
#define MAXMUDS 32	// Max number of muds allowed from one IP
#define ROUTER_BLACKLIST "/secure/daemon/i3router/blacklist.cfg" //bad IP's
#define DEB_IN  1	// //trr-Incoming
#define DEB_OUT 2	// //trr-Outgoing
#define DEB_INVALID 3	// //trr-Invalid
#define DEB_OTHER 0	// //trr-Other
#define DEBUGGER_GUY "cratylus"	// Name of who to display //trrging info to.
#define MAXIMUM_RETRIES 20
// SEND_WHOLE_MUDLIST makes it act like the official I3 server instead of like the I3 specs
#define SEND_WHOLE_MUDLIST
// SEND_WHOLE_CHANLIST makes it act like the official I3 server instead of like the I3 specs
#define SEND_WHOLE_CHANLIST
inherit LIB_DAEMON;

object cmd = load_object(CMD_ROUTER);
object rsocket = find_object(RSOCKET_D);

static void validate(){
    if( previous_object() != cmd && previous_object() != rsocket &&
      previous_object() != this_object() &&
      !((int)master()->valid_apply(({ "ASSIST" }))) ){
	trr("SECURITY ALERT: validation failure in ROUTER_D.","red");
	error("Illegal attempt to access router daemon: "+get_stack()+
	  " "+identify(previous_object(-1)));
    }
}

// Saved variables...
mapping listening;
// list of muds listening to each channel
// (key=chan name, value=mud array)
mapping connected_muds;
// muds that have successfully done a startup
// (key=mudname, value=fd)
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
static mapping muds_on_this_fd(int fd);
static mapping muds_not_on_this_fd(int fd);
void write_data(int fd, mixed data);
static void close_connection(int fd);
static void broadcast_data(mapping targets, mixed data);

// Ones with their own files...
string clean_fd(string fd);
static void broadcast_chanlist(string channame);
void broadcast_mudlist(string mudname);
static varargs void Debug(string str, int level);
static void process_channel(int fd, mixed *info);
static void process_startup_req(int protocol, mixed info, int fd);
static void read_callback(int fd, mixed info);
static void remove_mud(string mudname, int forced);
static void send_chanlist_reply(string mudname, int old_chanid);
static void send_mudlist(string mudname);
static void send_mudlist_updates(string updating_mudname, int old_mudlist_id);
static void send_startup_reply(string mudname);
static void send_error(string mud, string user, string errcode, string errmsg, mixed *info);
void send_full_mudlist(string mud);
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
varargs string *SetList();

// Code for all the stuff in the prototypes...
#include "./server_log.h"
#include "./irn.h"
#include "./clean_fd.h"
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
#include "./hosted_channels.h"
#include "./send_full_mudlist.h"

static void close_connection(int fd){
    RSOCKET_D->close_connection(fd);
}

void write_data(int fd, mixed data){
    RSOCKET_D->write_data(fd, data);
}

static void broadcast_data(mapping targets, mixed data){
    RSOCKET_D->broadcast_data(targets, data);
}

// debugging stuff...
mapping query_mudinfo(){ validate(); return copy(mudinfo); }
mapping query_mud(string str){ validate(); return copy(mudinfo[str]); }
mapping query_connected_muds(){ validate(); return copy(connected_muds); }
mapping query_socks(){ validate(); return RSOCKET_D->query_socks(); }
mapping query_chaninfo(){ return ([ "listening" : listening, "channels" : channels ]); }

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
    foreach(mixed element in socket_names()){
	if(intp(element[0]) && element[0] != -1 && !grepp(element[3],"*") &&
	  last_string_element(element[3],".") == router_port &&
	  member_array(element[0],
	    keys(this_object()->query_irn_sockets())) == -1) {
	    ret += ({ element[0] });
	}
    }
    return ret;
}

void get_info() {
    mixed *socky = sort_array(values(connected_muds), 1);
    mixed *muddy = sort_array(keys(connected_muds), 1);
    string socks = implode(socky, " ");
    string muddies = implode(muddy, ", ");
    int socknum = sizeof(socky);
    validate();

    socks += "\nTotal number of connected muds: "+socknum+"\n";
    write("router_name: "+router_name+
      "\nrouter_ip: "+router_ip+
      "\nrouter_port: "+router_port+
      "\nrouter_list"+identify(router_list)+
      "\nchannel_update_counter: "+ channel_update_counter+
      ((sizeof(channels)) ? "\nchannels:"+implode(sort_array(keys(channels),1),", ") : "")+
      "\nmudinfo_update_counter: "+ mudinfo_update_counter+
      "\nsockets: "+socks+
      "\nmuds: "+muddies+
      "\nRouter socket daemon uptime: "+
      time_elapsed(time()-RSOCKET_D->GetInceptDate())+
      ", up since "+ctime(RSOCKET_D->GetInceptDate())+
      "\n"+Report()
    );
}

void clear(){ 
    string mudname; 
    validate();
    server_log("%^RED%^Clearing all mud data!"); 
    foreach(mudname in keys(mudinfo)) remove_mud(mudname,1); 
    save_object(SAVE_ROUTER);    
}

string GetRouterName(){
    validate();
    return router_name;
}

string SetRouterName(string str){
    validate();
    if(first(str,1) != "*") str = "*"+str;
    router_name = str;
    server_log(" setting router name to: "+str); 
    SetList();
    return router_name;
}

string GetRouterIP(){
    validate();
    return router_ip;
}

string SetRouterIP(string str){
    validate();
    router_ip = str;
    server_log("Setting router IP to: "+str);
    SetList();
    return router_ip;
}

string GetRouterPort(){
    validate();
    return router_port;
}

string SetRouterPort(string str){
    validate();
    router_port = str;
    server_log("Setting router port to: "+str);
    SetList();
    return router_port;
}

string *GetRouterList(){
    validate();
    return router_list;
}

varargs string *SetList(){
    string tmp;
    string tmp_port = router_port;
    string tmp_ip = router_ip;
    validate();
    if(!strsrch(router_name,"*")) tmp = router_name;
    else tmp = "*"+router_name;
    if(lower_case(mud_name()) == "frontiers"){
	tmp_port = "23";
	tmp_ip = "149.152.218.102";
	tmp = "*yatmim";
    }
    router_list = ({ ({ tmp, tmp_ip+" "+tmp_port }) });
    save_object(SAVE_ROUTER);
    server_log("Setting router list to: "+identify(router_list));
    save_object(SAVE_ROUTER);
    return router_list;
}

varargs string *SetRouterList(string *str){
    string tmp;
    string tmp_port = router_port;
    string tmp_ip = router_ip;
    validate();
    return router_list;
    if(!strsrch(router_name,"*")) tmp = router_name;
    else tmp = "*"+router_name;
    if(!str || !sizeof(str)){
	if(lower_case(mud_name()) == "frontiers"){
	    tmp_port = "23";
	    tmp_ip = "149.152.218.102";
	    tmp = "*yatmim";
	}
	router_list = ({ ({ tmp, tmp_ip+" "+tmp_port }) });
	save_object(SAVE_ROUTER);
	return router_list;
    }
    router_list = ({ str });
    server_log("Setting router list to: "+identify(router_list));
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
    server_log(str+" has been BANNED");
    save_object(SAVE_ROUTER);
    return banned_muds;
}

string *RemoveBannedMud(string str){
    validate();
    banned_muds -= ({ str });
    server_log(str+" has been unbanned.");
    save_object(SAVE_ROUTER);
    return banned_muds;
}

string *GetBlacklistedMuds(){
    validate();
    return blacklisted_muds;
}

string *AddBlacklistedMud(string str){
    validate();
    blacklisted_muds += ({ str });
    server_log(str+" has been BLACKLISTED");
    save_object(SAVE_ROUTER);
    return blacklisted_muds;
}

string *RemoveBlacklistedMud(string str){
    validate();
    blacklisted_muds -= ({ str });
    server_log(str+" has been unblacklisted.");
    save_object(SAVE_ROUTER);
    return blacklisted_muds;
}

mixed GetRemoteIP(int fd){
    mixed conn = socket_status(fd);
    string ret;
    if(!conn || !sizeof(conn)) return 0;
    if(!conn[4] || conn[4] == "*.*") return 0;
    conn = explode(conn[4],".");
    ret = implode(conn[0..3],".");
    return ret;
}  

void check_discs(){
    int *fds = values(connected_muds);
    int i = 1;
    if(sizeof(fds))
	foreach(mixed element in sort_array(fds,1)){
	string lost_mud;
	if(!intp(element)) continue;
	if(!socket_status(element) ||
	  socket_status(element)[1] == "CLOSED" || !GetRemoteIP(element) ||
            GetRemoteIP(element) != mudinfo[query_connected_fds()[element]]["ip"]){
	    foreach(string key, mixed val in mudinfo){
		if(!connected_muds[key] && mudinfo[key]["router"] && mudinfo[key]["router"] == my_name){
		    server_log("Cleaning connection info from "+key);
		    if(!mudinfo[key]["disconnect_time"])
			mudinfo[key]["disconnect_time"] = time();
		    if(mudinfo[key]["connect_time"]) 
			mudinfo[key]["connect_time"] = 0;
		}
	    }

	    foreach(string key, int val in connected_muds){
		if(val == element){
		    trr("REMOVING DISCONNECTED: "+key+" from "+val);
		    i++;
		    call_out( (: disconnect_mud :), i, key);
		}
	    }
	}
    }
}

void clear_discs(){ 
    string mudname; 
    int i = 1;

    validate();

    foreach(mudname in keys(mudinfo)) {
	int deadsince = time() - mudinfo[mudname]["disconnect_time"];
	//tc(mudname+": "+ctime((mudinfo[mudname]["disconnect_time"]) || time()),"white");
	//tc("dead age: "+time_elapsed(deadsince),"white");
	if(mudinfo[mudname]["disconnect_time"] && deadsince > 604800 ){
	    //tc(mudname+": "+time_elapsed(deadsince),"red");
	    if(mudinfo[mudname]["router"] && mudinfo[mudname]["router"] == my_name){
		server_log("I want to remove "+mudname+". Its disconnect time is "+ctime(query_mud(mudname)["disconnect_time"]));
		server_log("Which was "+time_elapsed(time() - query_mud(mudname)["disconnect_time"])+" ago.");
		i = i+1;
		call_out( (: remove_mud :), i, mudname,1);
	    }
	    else {
		//server_log(mudname+" is not listed as a local mud.");
	    }
	}

	if(mudinfo[mudname] && mudinfo[mudname]["disconnect_time"] > 0 &&
	  mudinfo[mudname]["connect_time"] > 0){
	    i = 1;
	    server_log("I want to remove "+mudname+". It is in a paradox state.");
	    if(member_array(mudname,keys(query_connected_muds())) != -1){
		//trr("Its fd is: "+query_connected_muds()[mudname],"white");
	    }
	    else {
		//trr("It is not listed as a connected mud.","white");
	    }
	    server_log("Removing disconnected mud: "+identify(mudname));
	    i = i+2;
	    call_out( (: remove_mud :), i, mudname,1);
	}
    }
}

int eventDestruct(){
    validate();
    save_object(SAVE_ROUTER);
    server_log("I am being destructed by: \n"+get_stack()+
      "\n"+identify(previous_object(-1)));
    daemon::eventDestruct();
}

string query_fd_info(mixed foo){
    int num, i;
    string ret = "";    validate();
    if(stringp(foo)) if(sscanf(foo,"%d",num) != 1) return "foo";
    if(intp(foo)) num = foo;
    for(i=0;i<num;i++){
	mapping bar = query_connected_fds();
	ret += i+" "+socket_address(i)+" "+(bar[i]+"" || "")+"\n";
    }
    return ret;
}

int GetMaxRetries(){ return MAXIMUM_RETRIES; }

varargs void ReceiveList(mixed data, string type){
    string *cmuds = keys(connected_muds);
    //trr("ReceiveList: " +identify(keys(data)),"red");
    //trr("ReceiveList ("+type+"): " +identify(data),"red");
    //trr("Current muds: "+identify(keys(mudinfo)));
    if(!type || !sizeof(type)) type = "mudlist";
    if(!mapp(data)){
	//trr("router: ReceiveList: NOT A MAPPING: "+get_stack(),"red");
	return;
    }
    if(type == "mudlist"){
	foreach(mixed key, mixed val in data){
	    if(member_array(key,cmuds) != -1) continue;
	    if(mudinfo[key] && intp(val)){
		trr("ROUTER_D: deleting "+key);
		mudinfo[key]["disconnect_time"] = time();
		mudinfo[key]["connect_time"] = 0;
		//broadcast_mudlist(key, 1);
		//call_out( (: broadcast_mudlist :), 2 , key, 1 );
		schedule_broadcast(key, 1);
		map_delete(mudinfo, key);
		continue;
	    }
	    //trr("received remote list: "+key,"white");
	    if(!mapp(val)){
		//trr("VAL NOT A MAPPING. key: "+identify(key)+", val: "+identify(val),"blue");
		return;
	    }
	    //trr("accepting "+key+" into mudinfo mapping.","cyan");
	    mudinfo_update_counter++;
	    mudinfo_updates[key] = mudinfo_update_counter;
	    mudinfo[key]=val;
	    //broadcast_mudlist(key, 1);
	    //call_out( (: broadcast_mudlist :), 2 , key, 1 );
	    schedule_broadcast(key, 1);
	}
    }
    else if(type == "chanlist"){
	//trr("Current muds: "+identify(keys(mudinfo)));
	if(data["listening"] && sizeof(data["listening"]) && mapp(data["listening"])){
	    foreach(mixed key, mixed val in data["listening"]){
		//listening[key] = val;
		//trr("listening update: "+key+" is "+identify(val));
	    }
	}
	if(data["channels"] && sizeof(data["channels"]) && mapp(data["channels"])){
	    foreach(mixed key, mixed val in data["channels"]){
		string ownermud = data["channels"][key][1];
		if(member_array(ownermud, cmuds) != -1) continue;
		if(val == -1) map_delete(channels, key);
		if(val == -1) map_delete(listening, key);
		else {
		    val[2] = singular_array(val[2]);
		    channels[key] = val;
		}
		broadcast_chanlist(key);
		//trr("chan update: "+key+" is "+identify(val));
	    }
	}
    }
    save_object(SAVE_ROUTER);
}

int purge_crud(){
    foreach(mixed key, mixed val in mudinfo){
	if(!mapp(val)) map_delete(mudinfo, key);
    }
    save_object(SAVE_ROUTER);
    return sizeof(mudinfo);
}

varargs int purge_ip(string ip, int rude, mixed *sock_array){
    //tc("...received purge order.");
    validate();
    //tc("will I make it past socket_names?","yellow");
    if(!sock_array || !sizeof(sock_array)) sock_array = socket_names();
    //tc("YES!","green");
    foreach(mixed element in sock_array){
	int fd = element[0];
	if(last_string_element(element[3],".") != router_port) continue;
	if(member_array(fd, keys(irn_sockets)) != -1) continue;
	if(ip == "*") continue;
	//tc("so far so good.");
	if(clean_fd(socket_address(fd)) == ip){
	    //tc("mmhmm. dissing "+identify(element));
	    if(query_connected_fds()[fd]){
		if(rude){
		    server_log("router: fd to be rudely purged: "+fd+", "+query_connected_fds()[fd]);
		    disconnect_mud(query_connected_fds()[fd]);
		}
	    }
#if 0
	    else {
		server_log("router: wanting to purge fd:"+fd+", status: "+identify(element));
		close_connection(fd);
	    }
#endif
	}
    }
    //tc("GOAL!","green");
    return 1;
}

varargs int purge_ips(int rude){
    mixed *sockies = socket_names();
    foreach(mixed element in sockies){
	string ip_address = element[3];
	//tc("looking at: "+identify(element),"blue");
	if(last_string_element(ip_address,".") == router_port ){
	    //tc("I decide to purge.");
	    ip_address = replace_string(element[4],"."+last_string_element(element[4],"."),"");
	    //tc("purging: "+ip_address);
	    purge_ip(ip_address,(rude || 0), sockies);
	}
    }
    return 1;
}
