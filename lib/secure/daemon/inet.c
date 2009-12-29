/*    /secure/daemon/inet.c
 *    From the Dead Souls Mud Library
 *    A daemon to handle the start and stop of inet servers
 *    Created by Descartes of Borg 961219
 *    Version: @(#) inet.c 1.1@(#)
 *    Last modified: 96/12/19
 */

#include <lib.h>
#include <save.h>

inherit LIB_DAEMON;

class service {
    int    PortOffset;
    string SocketClass;
    int    SocketType;
}

private static mapping Servers  = ([]);
private mapping        Services = ([]);
mapping BlockedIps = ([]);

int eventStartServer(string svc);
int eventStopServer(string svc);

void validate(){
    if(!master()->valid_apply(({ "SECURE", "ASSIST" }))){
        string offender = identify(previous_object(-1));
        debug("INET_D SECURITY VIOLATION: "+offender+" ",get_stack(),"red");
        log_file("security", "\n"+timestamp()+" INET_D breach: "+offender+" "+get_stack());
        error("INET_D SECURITY VIOLATION: "+offender+" "+get_stack());
    }
}

void eventBlockIp(string ip){
    log_file("security", "\n"+timestamp()+" INET_D ip block: "+ip+"\n");
    if(ip != "*.*" && member_array(ip, keys(BlockedIps)) == -1){
        BlockedIps[ip] = time();
        eventSave();
    }
}

string *GetBlockedIps(){
    return keys(BlockedIps);
}

mapping eventClearBlockedIps(){
    validate();
    BlockedIps = ([]);
    log_file("security", "\n"+timestamp()+" INET_D all IP's unblocked.\n");
    eventSave();
    return copy(BlockedIps);
}

mapping GetServers() {
    return copy(Servers);
}

int GetServer(string str){
    if(Servers[str]) return 1;
    return 0;
}

int GetService(string str){
    if(Services[str]) return 1;
    return 0;
}

mapping GetServices() {
    return copy(Services);
}


int AddService(string name, int port_offset, string socket_class, int type) {
    class service s = new(class service);

    if( !master()->valid_apply(({})) ) {
        return 0;
    }
    s->PortOffset = port_offset;
    s->SocketClass = socket_class;
    s->SocketType = type;
    Services[name] = s;
    if( !eventSave() ) {
        return 0;
    }
    Servers[name] = new(LIB_SERVER, query_host_port()+port_offset, type,
            socket_class);
    return 1;
}

int RemoveService(string name) {
    if( !master()->valid_apply() ) {
        return 0;
    }
    if( Servers[name] ) {
        Servers[name]->eventShutdown();
    }
    map_delete(Servers, name);
    map_delete(Services, name);
    return eventSave();
}

varargs int eventRestartServer(string svc, int forced) {
    if( !eventStopServer(svc) ) {
        if(!forced) return 0;
    }
    if( !eventStartServer(svc) ) {
        if(!forced) return 0;
    }
    return 1;
}

int eventStartServer(string svc) {
    class service s = Services[svc];
    object ob = Servers[svc];

    if( !master()->valid_apply(({})) ) {
        return 0;
    }
    if( ob ) {
        return 0;
    }
    if( !s ) {
        return 0;
    }
    ob = new(LIB_SERVER, query_host_port()+s->PortOffset, s->SocketType,
            s->SocketClass);
    if( ob ) {
        Servers[svc] = ob;
        return 1;
    }
    else {
        return 0;
    }
}

int eventStopServer(string svc) {
    object ob = Servers[svc];

    if( !master()->valid_apply(({})) ) {
        return 0;
    }
    if( !ob ) {
        return 0;
    }
    ob->eventShutdown();
    map_delete(Servers, svc);
    if( ob ) {
        return 0;
    }
    else {
        return 1;
    }
}

static void create() {
    SetSaveFile(SAVE_INET);
    daemon::create();
    if(!BlockedIps) BlockedIps = ([]);
    SetNoClean(1);
    set_heart_beat(10);
    foreach(string svc, class service s in Services) {
        Servers[svc] = new(LIB_SERVER, query_host_port() + s->PortOffset,
                s->SocketType,  s->SocketClass);
    }
}

int eventDestruct(){
    object *servers = filter(objects(), (: base_name($1) == LIB_SERVER :) );
    eventSave();
    foreach(object server in servers){
        if(server && clonep(server)) server->eventDestruct();
        if(server && clonep(server)) destruct(server);
    }
    return daemon::eventDestruct();
}

void heart_beat(){
    validate();
    foreach(mixed key, mixed val in BlockedIps){
        if((time() - val) > 60) map_delete(BlockedIps, key);
    }
}

int GetServicePort(string str){
    int ret;
    int offset;
    if(Services[str]) offset = Services[str]->PortOffset;
    ret = query_host_port() + offset;
    return ret;
}
