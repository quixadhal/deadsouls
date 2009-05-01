/*
 * This is a sample server object. It doesn't do anything
 * as-is. It simply serves as a template for you to understand
 * how to build a server object of your own.
 */

#include NETWORK_H
int port = 789;

void StartServer();
void StopServer();

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
    debug("generic: listening.");
}

static void close_callback(int fd){
    debug("I'm wanting to close fd"+fd+" now.");
}

static void read_callback(int fd, buffer data){
    int i;
    string tmp="";
    debug("fd is: "+fd);
    for(i=0;i<sizeof(data);i++){
        tmp += sprintf("%c",data[i]);
    }
    debug("tmp: "+tmp);
    debug("data: "+identify(read_buffer(data)));
    return;
}
