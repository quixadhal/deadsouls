/*
 * This is a sample uptime server. All it does is tell you
 * how long the mud's been up, then disconnects the telnet
 * session. If your mud game port is 6666, this will be
 * on 6668.
 */

#include NETWORK_H
int port = query_host_port()+2;

void StartServer();
int SendUptime(int fd);

void create(){
    StartServer();
}

void StartServer(){
    int sockstat, listsock;

    listsock = socket_create(STREAM,"read_callback","close_callback");
    if(listsock < 0){
        debug("Couldn't create socket. errorcode: "+listsock);
        return;
    }

    sockstat = socket_bind(listsock,port);
    if(sockstat < 0){
        debug("Couldn't bind socket. errorcode: "+sockstat);
        return;
    }

    sockstat = socket_listen(listsock,"listen_callback");
    if(sockstat < 0){
        debug("Couldn't listen on socket. errorcode: "+sockstat);
        return;
    }

}

static void listen_callback(int fd){
    int sockstat = socket_accept(fd,"read_callback","write_callback");

    if(sockstat < 0){
        debug("Couldn't accept on socket. errorcode: "+sockstat);
        return;
    }

    debug("uptime server: listening.");
    SendUptime(sockstat);
}

static void close_callback(int fd){
    debug("I'm wanting to close fd"+fd+" now.");
}

static mixed read_callback(int fd, mixed data){
    debug("quitting. fd: "+fd+", "+identify(socket_status(fd)));
    debug(socket_close(fd));
    return 1;
}

int SendUptime(int fd){
    socket_write(fd, "\n"+mud_name()+" has been up "+time_elapsed(uptime())+"\n\n");
    debug("socket_close: "+socket_close(fd));
    return 1;
}
