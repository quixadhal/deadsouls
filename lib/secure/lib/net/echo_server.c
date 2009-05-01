/*
 * This is a simple echo server. It just spews out what the
 * client types in. To disconnect the client types: quit
 * If your mud port is 6666, this will be on 6668
 */

#include NETWORK_H
int port = PORT_ECHO;

void StartServer();

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

    debug("echo server: listening.");
}

static void close_callback(int fd){
    debug("I'm wanting to close fd"+fd+" now.");
}

static mixed read_callback(int fd, mixed data){
    int i;
    string tmp="";
    mixed tmp2;
    debug("fd is: "+fd);

    for(i=0;i<sizeof(data);i++){
        if(data[i] == 13) continue;
        tmp2 = sprintf("%c",data[i]);
        tmp += tmp2; 
    }
    tmp2 = truncate(tmp,1);
    if(bufferp(data)) debug("data (buffer): "+identify(read_buffer(data)));
    else debug("data: "+identify(data));
    socket_write(fd, "I read: "+tmp);    
    if(tmp2 == "quit"){
        debug("quitting");
        i = socket_close(fd);
        debug("i: "+i);
    }
    return i;
}
