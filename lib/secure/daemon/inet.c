/*    /secure/daemon/inet.c
 *    From the Dead Souls V Object Library
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

int eventStartServer(string svc);
int eventStopServer(string svc);

mapping GetServers() {
    return copy(Servers);
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

int eventRestartServer(string svc) {
    if( !eventStopServer(svc) ) {
	return 0;
    }
    if( !eventStartServer(svc) ) {
	return 0;
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
    SetNoClean(1);
    foreach(string svc, class service s in Services) {
	Servers[svc] = new(LIB_SERVER, query_host_port() + s->PortOffset,
			   s->SocketType,  s->SocketClass);
    }
}
