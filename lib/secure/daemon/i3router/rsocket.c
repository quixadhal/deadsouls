#include <lib.h>
#include <commands.h>
#include <socket.h>
#include <daemons.h>
#include <network.h>

static private int router_socket;
static private mapping sockets = ([]);


void write_data(int fd, mixed data);
object cmd = load_object(CMD_ROUTER);
object router = find_object(ROUTER_D);

static void validate(){
    if( previous_object() != cmd && previous_object() != router &&
      !((int)master()->valid_apply(({ "ASSIST" }))) )
	error("Illegal attempt to access router socket daemon: "+get_stack()+
	  " "+identify(previous_object(-1)));
}

static void create(){
    call_out("setup",1);
}

void close_connection(int fd){
    int sockerr;

    validate();

    if(socket_status(fd)[1] == "LISTEN") return;
    trr("About to try closing socket: "+fd,"yellow");
    trr("Pre-closing state: "+socket_status(fd)[1],"yellow");
    sockerr = socket_close(fd);
    if(sockerr > -1) map_delete(sockets,fd);
    trr("closing socket:"+fd,"white");
    trr("closing sockerr:"+sockerr,"white");
    trr("Post-closing state: "+socket_status(fd)[1],"yellow");
}

static void close_callback(int fd){
    string mudname;
    mapping muds_on_this_fd = ([]);

    if(!find_object(ROUTER_D)) return;

    muds_on_this_fd = ROUTER_D->query_connected_muds();
    foreach(mixed key, mixed val in muds_on_this_fd){
	if(val != fd) map_delete(muds_on_this_fd, key);
    }
    trr("close_callback: fd="+fd+"\n");
    if(socket_status(fd)[1] == "LISTEN") return;
    foreach(mudname in keys(muds_on_this_fd)){
	trr(timestamp()+" close_callback: Removing mud from connected_muds list: "+mudname,"red");
	ROUTER_D->disconnect_mud(mudname);
    }
    close_connection(fd);
}

static void listen_callback(int fd){
    int fdstat;
    //trr("listen_callback: socket_status("+fd+"): "+identify(socket_status(fd)));
    if ((fdstat = socket_accept(fd, "read_callback", "write_callback")) < 0) {
	//trr("listen_callback couldn't accept socket "+fd+", errorcode "+fdstat);
	return;
    }
    //else trr("listen_callback: fdstat: "+fdstat);
}

static void read_callback(int fd, mixed info){
    if(!find_object(ROUTER_D)) return;
    ROUTER_D->read_callback(fd,info);
}

static void write_callback(int fd){
    if(!sockets[fd]) return;
    if(sockets[fd]["write_status"] == EEALREADY) {
	write_data(fd, sockets[fd]["pending"]);
	map_delete(sockets[fd], "pending");
    } 
    else {
	sockets[fd]["write_status"] = EESUCCESS;
    }
}

static void write_data_retry(int fd, mixed data, int counter){
    int rc;
    int maxtry;

    if(!find_object(ROUTER_D)) return;

    maxtry = ROUTER_D->GetMaxRetries();
    if (counter == maxtry) {
	trr("Could not write data to "+this_object()->query_connected_fds()[fd]+
	  " "+identify(data));
	return;
    }
    rc = socket_write(fd, data);
    if(!sockets[fd]){
	sockets[fd]=([]);
    }
    sockets[fd]["write_status"] = rc;
    switch (rc) {
    case EESUCCESS:
	break;
    case EEALREADY:
	sockets[fd]["pending"] = data;
	break;
    case EECALLBACK:
	break;
    default:
	if (counter < maxtry) {
	    trr("write_data_retry: " + socket_error(rc));
	    call_out( (: write_data_retry :), 2 , fd, data, counter + 1 ); 
	    return;
	}
    }
}

void write_data(int fd, mixed data){
    validate();
    write_data_retry(fd, data, 0);
}

void broadcast_data(mapping targets, mixed data){
    validate();
    foreach(int *arr in unique_array(values(targets), (: $1 :))){
	write_data(arr[0], data);
    }
}

static void setup(){
    int router_port;

    if(!find_object(ROUTER_D)) return;

    router_port = atoi(ROUTER_D->GetRouterPort());
    trr("setup got called");
    log_file("router/server_log",timestamp()+" setup has been called.\n");
    if ((router_socket = socket_create(MUD, "read_callback", "close_callback")) < 0){
	log_file("router/server_log", timestamp()+"setup: Failed to create socket.\n");
	trr("setup: Failed to create socket.\n");
	return;
    }
    if (socket_bind(router_socket, router_port) < 0) {
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
}

mapping query_socks(){ validate(); return copy(sockets); }
