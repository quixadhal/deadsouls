#include NETWORK_H
#include <cfg.h>

inherit LIB_DAEMON;

int port = PORT_FLASH;
string policy_file = "no policy file";

void StartServer();

void create(){
    if(file_exists(CFG_FLASH_POLICY)){
        policy_file = read_file(CFG_FLASH_POLICY);
    }
    StartServer();
}

void StartServer(){
    int sockstat, listsock;

    listsock = socket_create(STREAM,"read_callback","close_callback");
    if(listsock < 0){
        debug("flash Couldn't create socket. errorcode: "+listsock);
        return;
    }

    sockstat = socket_bind(listsock,port);
    if(sockstat < 0){
        debug("flash Couldn't bind socket. errorcode: "+sockstat);
        return;
    }

    sockstat = socket_listen(listsock,"listen_callback");
    if(sockstat < 0){
        debug("flash Couldn't listen on socket. errorcode: "+sockstat);
        return;
    }

}

static void listen_callback(int fd){
    int sockstat = socket_accept(fd,"read_callback","write_callback");

    if(sockstat < 0){
        debug("flash Couldn't accept on socket. errorcode: "+sockstat);
        return;
    }

    debug("flash echo server: listening.");
}

static void close_callback(int fd){
    debug("flash I'm wanting to close fd"+fd+" now.");
}

static mixed read_callback(int fd, mixed data){
    int i;
    string tmp="";
    mixed tmp2;
    debug("flash fd is: "+fd);

    for(i=0;i<sizeof(data);i++){
        if(data[i] == 13) continue;
        tmp2 = sprintf("%c",data[i]);
        tmp += tmp2; 
    }
    tmp2 = truncate(tmp,1);
    if(bufferp(data)) debug("flash data (buffer): "+identify(read_buffer(data)));
    else debug("flash data: "+identify(data));
    if(!strsrch(data,"<policy-file-request/>")){
        debug("Flash request detected.");
        socket_write(fd, policy_file);
    }
    debug("flash closing socket "+fd);
    i = socket_close(fd);
    debug("flash i: "+i);
    return i;
}
