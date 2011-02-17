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
int verbose;
int heart_count, interval = 1;
int last_connect = time();
static int *badsocks = ({});

void write_data(int fd, mixed data);
varargs void yenta(mixed arg1, mixed arg2);
object cmd = load_object(CMD_ROUTER);
object router = find_object(ROUTER_D);

varargs static void validate(int i){
    if(i){
        if(!socket_status(i) || !socket_status(i)[5]){
            server_log("%^RED%^BAD SOCKET ALERT. fd "+i+":  "+
                    identify(socket_status(i)),"rsocket");
            error("Bad socket, fd "+i);
        }
    }
    if( previous_object() != cmd && previous_object() != router &&
            previous_object() != this_object() && !((int)master()->valid_apply(({ "ASSIST" }))) ){
        server_log("%^RED%^SECURITY ALERT: validation failure in RSOCKET_D.","rsocket");
        error("Illegal attempt to access router socket daemon: "+get_stack()+
                " "+identify(previous_object(-1)));
    }
}

mixed SetBadsocks(mixed foo){
    if(arrayp(foo)) badsocks = copy(foo);
    else badsocks = ({ });
    return badsocks;
}

static void create(){
    incept_date = time();
    call_out("setup",1);
    SetNoClean(1);
    set_heart_beat(1);
}

void heart_beat(){
    heart_count++;
    if(!(heart_count % 60)){
        SetBadsocks( ({}) );
    }
    if(heart_count > 300){
        heart_count = 0;
    }
}

void close_connection(int fd){
    int sockerr;
    mixed *sockstat = ({});

    validate();

    sockstat = socket_status(fd);
    if(!sockstat || !sizeof(sockstat)) return;
    if(sockstat[1] == "LISTEN") return;
    server_log("%^YELLOW%^About to try closing socket: "+fd+
            " (aka: "+(ROUTER_D->query_connected_fds()[fd] ||
                    identify(sockstat))+")","rsocket");
    yenta("%^YELLOW%^Pre-closing state: "+sockstat[1],"rsocket");
    sockerr = socket_close(fd);
    if(sockerr > -1) map_delete(sockets,fd);
    badsocks -= ({ fd });
    yenta("%^WHITE%^closing socket:"+fd,"rsocket");
    yenta("%^WHITE%^closing sockerr:"+sockerr,"rsocket");
    yenta("%^YELLOW%^Post-closing state: "+socket_status(fd)[1],"rsocket");
    yenta("%^WHITE%^---\n","rsocket");
}

static void close_callback(int fd){
    string mudname;
    mapping muds_on_this_fd = ([]);

    validate(fd);

    if(!find_object(ROUTER_D)) return;

    ROUTER_D->close_notify(fd);

    muds_on_this_fd = ROUTER_D->query_connected_muds();
    foreach(mixed key, mixed val in muds_on_this_fd){
        if(val != fd) map_delete(muds_on_this_fd, key);
    }
    yenta("close_callback: fd="+fd+"\n");
    if(socket_status(fd)[1] == "LISTEN") return;
    foreach(mudname in keys(muds_on_this_fd)){
        server_log("%^RED%^close_callback: Removing mud from connected_muds list: "+(mudname || identify(socket_status(fd))),"rsocket");
        ROUTER_D->disconnect_mud(mudname);
    }
    close_connection(fd);
}

static void listen_callback(int fd){
    mixed fdstat,newfd;
    validate();

    if ((newfd = socket_accept(fd, "read_callback", "write_callback")) < 0) {
        trr("listen_callback couldn't accept socket "+fd+", errorcode "+newfd);
        return;
    }
    else {
        server_log("socket_accepted: "+newfd+
                ", "+identify(socket_status(newfd)),"rsocket");

        if(sizeof(fdstat) > 4){
            foreach(mixed foo in ROUTER_D->GetBlacklistedMuds()){
                if(!strsrch(fdstat[4]+"",foo)){
                    badsocks += ({ newfd });
                    return;
                }
            }
        }

        if(time() - last_connect < interval){
            badsocks -= ({ newfd });
            socket_close(newfd);
            return;
        }

        last_connect = time();
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
    if(!find_object(ROUTER_D)) return;
    if(member_array(fd, badsocks) != -1) return;
    ROUTER_D->read_callback(fd,info);
}

static void write_callback(int fd){

    validate(fd);

    if(member_array(fd, badsocks) != -1) return;

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

    if(member_array(fd, badsocks) != -1) return;

    if(!find_object(ROUTER_D)) return;

    maxtry = ROUTER_D->GetMaxRetries();
    if (counter == maxtry) {
        trr("Could not write data to "+ROUTER_D->query_connected_fds()[fd]+", fd"+fd+": "+identify(data[0]));
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
                    trr("RSOCKET_D write_data_retry "+counter+" to "+
                            ROUTER_D->query_connected_fds()[fd]+", fd"+fd+" error,  code "+rc+": " + socket_error(rc));
                call_out( (: write_data_retry :), 2 , fd, data, counter + 1 ); 
                return;
            }
    }
}

void write_data(int fd, mixed data){
    validate(fd);
    if(member_array(fd, badsocks) != -1) return;
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
    server_log("rsocket setup got called","rsocket");
    if ((router_socket = socket_create(MUD, "read_callback", "close_callback")) < 0){
        server_log("setup: Failed to create socket.","rsocket");
        return;
    }
    if (socket_bind(router_socket, router_port) < 0) {
        socket_close(router_socket);
        server_log("setup: Failed to bind socket to port.","rsocket");
        return;
    }
    if (socket_listen(router_socket, "listen_callback") < 0) {
        socket_close(router_socket);
        server_log("setup: Failed to listen to socket.","rsocket");
    }
    server_log("rsocket setup ended","rsocket");
}

void complete_socket_handoff(int i){

    validate(i);

    yenta("hit the right handoff fun. arg: "+i);
    if(base_name(previous_object()) != ROUTER_D){
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
    string alert = "RSOCKET_D is being killed!\n";
    alert += "STACK: "+get_stack()+"\n";
    alert += "PREVS: "+identify(previous_object(-1))+"\n";

    server_log("%^RED%^"+alert,"rsocket");
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
