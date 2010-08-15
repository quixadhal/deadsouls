#include <lib.h>
#include <commands.h>
#include <socket.h>
#include <daemons.h>
#include NETWORK_H
#include "./server_log.h"

inherit LIB_DAEMON;

static private int router_socket;
static private mapping sockets = ([]);
static private int incept_date;
int verbose = 0;

static void close_callback(int fd, int force);
void write_data(int fd, mixed data);
varargs void yenta(mixed arg1, mixed arg2);
object cmd = load_object(CMD_ROUTER);
object router = find_object(IMC2_SERVER_D);

varargs static void validate(int i){
    if(i){
        if(!socket_status(i) || !socket_status(i)[5]){
            server_log("%^RED%^BAD SOCKET ALERT. fd "+i+":  "+
                    identify(socket_status(i)),"ssocket");
            error("Bad socket, fd "+i);
            close_callback(i, 1);
        }
    }
    if( previous_object() != cmd && previous_object() != router &&
            previous_object() != this_object() && !((int)master()->valid_apply(({ "ASSIST" }))) ){
        server_log("%^RED%^SECURITY ALERT: validation failure in SSOCKET_D.","ssocket");
        error("Illegal attempt to access router socket daemon: "+get_stack()+
                " "+identify(previous_object(-1)));
    }
}

static void create(){
    incept_date = time();
    call_out("setup",1);
    SetNoClean(1);
}

void close_connection(int fd){
    int sockerr;
    mixed *sockstat = ({});

    validate();

    sockstat = socket_status(fd);
    if(!sockstat || !sizeof(sockstat)) return;
    if(sockstat[1] == "LISTEN") return;
    server_log("%^YELLOW%^About to try closing socket: "+fd
            //+" (aka: "+(ROUTER_D->query_connected_fds()[fd] ||
            //  identify(sockstat))+")","ssocket"
            );
    yenta("%^YELLOW%^Pre-closing state: "+sockstat[1],"ssocket");
    sockerr = socket_close(fd);
    if(sockerr > -1) map_delete(sockets,fd);
    yenta("%^WHITE%^closing socket:"+fd,"ssocket");
    yenta("%^WHITE%^closing sockerr:"+sockerr,"ssocket");
    yenta("%^YELLOW%^Post-closing state: "+socket_status(fd)[1],"ssocket");
    yenta("%^WHITE%^---\n","ssocket");
}

static void close_callback(int fd, int force){
    string mudname;
    mapping muds_on_this_fd = ([]);

    if(!force) validate(fd);

    if(!find_object(ROUTER_D)) return;

    muds_on_this_fd = ROUTER_D->query_connected_muds();
    foreach(mixed key, mixed val in muds_on_this_fd){
        if(val != fd) map_delete(muds_on_this_fd, key);
    }
    yenta("close_callback: fd="+fd+"\n");
    if(socket_status(fd)[1] == "LISTEN") return;
    foreach(mudname in keys(muds_on_this_fd)){
        server_log("%^RED%^close_callback: Removing mud from connected_muds list: "+(mudname || identify(socket_status(fd))),"ssocket");
        ROUTER_D->disconnect_mud(mudname);
    }
    close_connection(fd);
}

static void listen_callback(int fd){
    int fdstat;

    validate(fd);

    if ((fdstat = socket_accept(fd, "read_callback", "write_callback")) < 0) {
        trr("listen_callback couldn't accept socket "+fd+", errorcode "+fdstat);
        return;
    }
    else {
        server_log("socket_accepted: "+fdstat+
                ", "+identify(socket_status(fdstat)),"ssocket");
    }
}

static void read_callback(int fd, mixed info){

    validate(fd);
    if(bufferp(info)){
        yenta("%^WHITE%^fd "+fd+" is sending me buffer data!");
        yenta("%^WHITE%^As far as I can tell, it is:");
        yenta("%^BLUE%^"+identify(read_buffer(info)));
    }
    else yenta("%^WHITE%^data from fd "+fd+":\n%^BLUE%^"+identify(info));
    if(!find_object(IMC2_SERVER_D)) return;
    IMC2_SERVER_D->read_callback(fd,info);
}

static void write_callback(int fd){

    validate(fd);

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

    validate(fd);
    //trr("counter for fd("+fd+"): "+counter);

    if(!find_object(IMC2_SERVER_D)) return;

    maxtry = IMC2_SERVER_D->GetMaxRetries();
    if (counter == maxtry) {
        trr("Could not write data to "+ROUTER_D->query_connected_fds()[fd]+", fd"+fd+": "+identify(data[0]));
        return;
    }
    if(!grepp(data,"close-notify") && !grepp(data,"is-alive")){
        yenta("%^YELLOW%^SSOCKET: bout to try writing "+identify(data)+" to "+fd,"ssocket");
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
        case EESECURITY:
            break;
        case EEFDRANGE:
            break;
        case EENOTCONN:
            break;
        case EEBADF:
            break;
        default:
            if (counter < maxtry) {
                if(counter < 2 || counter > maxtry-1)
                    trr("SSOCKET_D write_data_retry "+counter+" to "+
                            ROUTER_D->query_connected_fds()[fd]+", fd"+fd+" error,  code "+rc+": " + socket_error(rc));
                call_out( (: write_data_retry :), 2 , fd, data, counter + 1 ); 
                return;
            }
    }
}

void write_data(int fd, mixed data){
    validate(fd);
    write_data_retry(fd, data, 0);
}

void broadcast_data(mapping targets, mixed data){
    validate();
    yenta("SSOCK: broadcast_data("+identify(targets)+", "+identify(data));
    foreach(int *arr in unique_array(values(targets), (: $1 :))){
        write_data(arr[0], data);
    }
}

static void setup(){
    int router_port;

    if(!find_object(IMC2_SERVER_D)) return;

    router_port = atoi(IMC2_SERVER_D->GetRouterPort());
    server_log("ssocket setup got called","ssocket");
    if ((router_socket = socket_create(STREAM, "read_callback", "close_callback")) < 0){
        server_log("setup: Failed to create socket.","ssocket");
        return;
    }
    if (socket_bind(router_socket, router_port) < 0) {
        socket_close(router_socket);
        server_log("setup: Failed to bind socket to port.","ssocket");
        return;
    }
    if (socket_listen(router_socket, "listen_callback") < 0) {
        socket_close(router_socket);
        server_log("setup: Failed to listen to socket.","ssocket");
    }
    server_log("ssocket setup ended","ssocket");
}

void complete_socket_handoff(int i){

    validate(i);

    yenta("hit the right handoff fun. arg: "+i);
    if(base_name(previous_object()) != IMC2_SERVER_D){
        yenta("I don't want your dirty socket, "+identify(previous_object()));
        return;
    }
    yenta("sockstat: "+identify(socket_status(i)));
    socket_acquire(i, "read_callback", "write_callback", "close_callback");
    yenta("sockstat: "+identify(socket_status(i)));
}

mapping query_socks(){ validate(); return copy(sockets); }

int GetInceptDate(){
    return incept_date;
}

int eventDestruct(){
    string alert = "SSOCKET_D is being killed!\n";
    alert += "STACK: "+get_stack()+"\n";
    alert += "PREVS: "+identify(previous_object(-1))+"\n";

    server_log("%^RED%^"+alert,"ssocket");
    return ::eventDestruct();
}

int SetVerbose(int i){
    if(i) verbose = 1;
    else verbose = 0;
    return verbose;
}

int GetVerbose(){
    return verbose;
}

varargs void yenta(mixed arg1, mixed arg2){
    if(verbose){ 
        if(arg2) server_log(arg1, arg2);
        else server_log(arg1);
    }
}
