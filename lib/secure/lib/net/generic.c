#include <network.h>
int port = 6667;

void StartServer();
void StopServer();

void create(){
    StartServer();
}

void StartServer(){
    int sockstat, listsock;

    listsock = socket_create(STREAM,"read_callback","close_callback");
    if(listsock < 0){
	tc("Couldn't create socket. errorcode: "+listsock);
	return;
    }

    sockstat = socket_bind(listsock,port);
    if(sockstat < 0){
	tc("Couldn't bind socket. errorcode: "+sockstat);
	return;
    }

    sockstat = socket_listen(listsock,"listen_callback");
    if(sockstat < 0){
	tc("Couldn't listen on socket. errorcode: "+sockstat);
	return;
    }

}

static void listen_callback(int fd){
    int sockstat = socket_accept(fd,"read_callback","write_callback");

    if(sockstat < 0){
	tc("Couldn't accept on socket. errorcode: "+sockstat);
	return;
    }
    tc("generic: listening.");
}

static void close_callback(int fd){
    tc("I'm wanting to close fd"+fd+" now.");
}

static void read_callback(int fd, buffer data){
    int i;
    string tmp="";
    tc("fd is: "+fd);
    for(i=0;i<sizeof(data);i++){
	tmp += sprintf("%c",data[i]);
    }
    tc("tmp: "+tmp);
    tc("data: "+identify(read_buffer(data)));
    return;
}
