/*    /secure/lib/ftp_client.c
 *    From the Dead Souls Object Library
 *    a TCP client object
 *    Created by Descartes of Borg 950428
 *    Modified by Dvarsk to add read,write and close settable functions.
 *    Version: @(#) client.c 1.5@(#)
 *    Last modified: 97/03/31
 */

#include <lib.h>
#include <network.h>

inherit LIB_DAEMON;

class client {
    int Descriptor;
    int Blocking;
    int NoDestruct;
    mixed array Buffer;
}

private static int DestructOnClose, SocketType = -1;
private static string LogFile;
private static function Read;
private static function Write;
private static function Close;
private static class client Socket;

varargs static void eventClose(class client sock, int aboted);
static void eventRead(mixed val);
static void eventSocketClose();
static void eventSocketError(string str, int x);
int eventWriteDestruct();

function SetRead(function f) { return (Read = f); }

function SetWrite(function f) { return (Write = f); }

function SetClose(function f) { return (Close = f); }

int SetSocketType(int type) { return (SocketType = type); }

int SetDestructOnClose(int x) { return (DestructOnClose = x); }

int eventCreateSocket(string host, int port) {
    int x;

    Socket = new(class client);
    Socket->Blocking = 1;
    if( SocketType == -1 ) SocketType = MUD;
    x = socket_create(SocketType, "eventReadCallback", "eventAbortCallback");
    if( x < 0 ) {
	eventSocketError("Error in socket_create().", x);
	return x;
    }
    Socket->Descriptor = x;
    x = socket_bind(Socket->Descriptor, 0);
    if( x != EESUCCESS ) {
	eventClose(Socket);
	eventSocketError("Error in socket_bind().", x);
	return x;
    }
    x = socket_connect(Socket->Descriptor, host + " " + port, 
		       "eventReadCallback", "eventWriteCallback");
    if( x != EESUCCESS ) {
	eventClose(Socket);
	eventSocketError("Error in socket_connect().", x);
	return x;
    }
}

static void eventAbortCallback(int fd) {
    if( !Socket ) return;
    if( fd != Socket->Descriptor ) return;
    eventClose(Socket, 1);
}

static void eventReadCallback(int fd, mixed val) {
    if( functionp(Read) ) evaluate(Read, val);
    else eventRead(val);
}

static void eventRead(mixed val) { }

static void eventWriteCallback(int fd) {
    int x;

    if( !Socket ){
        eventDestruct();
        return;
    }
    if ( !sizeof(Socket->Buffer) && Write && Socket->Blocking ){
        Socket->Buffer = ({ evaluate(Write, this_object()) });
    }

    Socket->Blocking = 0;
    Socket->NoDestruct = 1;
    x = EESUCCESS;
    while( Socket->Buffer && x == EESUCCESS ) {
	switch( x = socket_write(Socket->Descriptor, Socket->Buffer[0]) ) {
            case EESUCCESS:
                if (Write ){
                    mixed tmp;

                    tmp = evaluate(Write, this_object());
                    if ( sizeof(tmp) ){
                        Socket->Buffer += ({ tmp });
                    }
                }
                Socket->NoDestruct = 0;
            break;
	    case EECALLBACK:
		Socket->Blocking = 1;
                Socket->NoDestruct = 1;
                break;
	    case EEWOULDBLOCK:
		call_out( (: eventWriteCallback($(fd)) :), 0);
                Socket->NoDestruct = 1;
		return;
	    case EEALREADY:
		Socket->Blocking = 1;
                eventDestruct();
		return;
	    default:
		eventClose(Socket);
		eventSocketError("Error in socket_write().", x);
                eventDestruct();
		return ;
	}
	if( sizeof(Socket->Buffer) == 1 ) Socket->Buffer = 0;
	else Socket->Buffer = Socket->Buffer[1..];

    } 
    eventWriteDestruct();
}

void eventWrite(mixed val) {
    int x;

    if( !Socket ) return;
    if( Socket->Buffer ) Socket->Buffer += ({ val });
    else Socket->Buffer = ({ val });
    if( Socket->Blocking ) return;
    else eventWriteCallback(Socket->Descriptor);
}

varargs static int eventClose(class client sock, int aborted) {
    if( !sock ) return 0;
    if( !aborted && socket_close(sock->Descriptor) != EESUCCESS ) {
        return 0;
    }
    sock = 0;
    if( functionp(Close) ) evaluate(Close);
    else eventSocketClose();
    if( DestructOnClose ) Destruct();
    return 1;
}

static void eventSocketClose() { }

int eventDestruct() {
    eventClose(Socket);
    return daemon::eventDestruct();
}

int eventWriteDestruct() {
    if(Socket->NoDestruct) return 0;
    return eventDestruct();
}

static void eventSocketError(string str, int x) { 
    if( LogFile ) 
      log_file(LogFile, ctime(time()) + "\n" + socket_error(x) + "\n");
}

