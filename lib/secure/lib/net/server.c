/*    /lib/net/server.c
 *    From the Dead Souls LPC Library
 *    a TCP server object
 *    created by Descartes of Borg 950429
 *    Version: @(#) server.c 1.3@(#)
 *    Last modified: 96/12/19
 */

#include <lib.h>
#include <daemons.h>
#include <network.h>
#include <runtime_config.h>

inherit LIB_DAEMON;

class server {
    int         Descriptor;
    int         Blocking;
    int         Closing;
    object      Owner;
    mixed array Buffer;
}

private static int          DestructOnClose= 0;
private static class server Listen         = 0;
private static int          MaxBytes       = get_config(__MAX_BYTE_TRANSFER__);
private static int          Port           = 0;
private static string       SocketObject   = 0;
private static int          SocketType     = STREAM;
private static mapping      Sockets        = ([]);

static void eventSocketError(string msg, int code);

/* ******************* server.c attributes ************************ */
int GetDestructOnClose() {
    return DestructOnClose;
}

static int SetDestructOnClose(int x) {
    return (DestructOnClose = x);
}

int GetSocketType() {
    return SocketType;
}

static int SetSocketType(int x ) {
    return (SocketType = x);
}

/* ******************** server.c events *************************** */
static int eventClose(mixed sock) {
    class server s;

    if( intp(sock) ) {
	sock = Sockets[sock];
    }
    else if( objectp(sock) ) {
	sock = Sockets[sock->GetDescriptor()];
    }
    if( !sock ) {
	return 0;
    }
    s = sock;
    if( s->Blocking ) {
	s->Closing = 1;
	return 1;
    }
    if( Sockets[s->Descriptor] ) {
	map_delete(Sockets, s->Descriptor);
    }
    socket_close(s->Descriptor);
    //    eventSocketClosed(s->Owner);
    if( s->Owner ) {
	s->Owner->eventSocketClosed();
    }
    if( DestructOnClose && sock == Listen ) {
	Destruct();
    }
    sock = 0;
}

int eventCreateSocket(int port) {
    int x;
    Listen = new(class server);
    Listen->Blocking = 0; /* servers are not blocking to start */
    x = socket_create(SocketType,
		      "eventServerReadCallback", 
		      "eventServerAbortCallback");
    if( x < 0 ) {
	eventSocketError("Error in socket_create().", x);
	return x;
    }
    Listen->Descriptor = x;
    x = socket_bind(Listen->Descriptor, port);
    if( x != EESUCCESS ) {
	eventClose(Listen);
	eventSocketError("Error in socket_bind().", x);
	return x;
    }
    x = socket_listen(Listen->Descriptor, "eventServerListenCallback");
    if( x != EESUCCESS ) {
	eventClose(Listen);
	eventSocketError("Error in socket_listen().", x);
	return x;
    }
}

static int Destruct() {
    if( daemon::Destruct() ) {
	foreach(int fd, class server socket in Sockets) {
	    socket->Owner->evenShutdown();
	}
	eventClose(Listen);
	return 1;
    }
    else {
	return 0;
    }
}

int eventDestruct() {
    if( !master()->valid_apply(({})) ) {
	return 0;
    }
    return daemon::eventDestruct();
}

static void eventNewConnection(object socket) {
    class server s = new(class server);
    s->Descriptor = socket->GetDescriptor();
    s->Blocking = 0;
    s->Owner = socket;
    Sockets[s->Descriptor] = s;
    socket->StartService(); // added for welcome
}

static void eventServerAbortCallback(int fd) {
    eventClose(fd);
}

int eventShutdown() {
    if( previous_object() != find_object(INET_D) ) {
	return 0;
    }
    Destruct();
    return 1;
}

static void eventServerListenCallback(int fd) {
    int x;

    x = socket_accept(fd,
		      "eventServerReadCallback", 
		      "eventServerWriteCallback");
    if( x < 0 ) {
	eventSocketError("Error in socket_accept().", x);
	return;
    }
    if( Sockets[x] ) {
	eventClose(Sockets[x]);
    }
    eventNewConnection(new(SocketObject, x, this_object()));
}

static void eventServerReadCallback(int fd, mixed val) {
    class server s = Sockets[fd];

    if( !s || !s->Owner ) {
	eventClose(fd);
	return;
    }
    else {
	s->Owner->eventRead(val);
    }
}

static void eventServerWriteCallback(int fd) {
    class server sock;
    int x;

    if( Listen && Listen->Descriptor == fd ) {
	sock = Listen;
    }
    else if( Sockets[fd] ) {
	sock = Sockets[fd];
    }
    else {
	return;
    }
    sock->Blocking = 0;
    if( !sock->Buffer && sock->Closing ) {
	eventClose(sock);
	return;
    }
    x = EESUCCESS;
    while( sock->Buffer && x == EESUCCESS ) {
	switch( x = socket_write(sock->Descriptor, sock->Buffer[0]) ) {
            case EESUCCESS:
	        break;
	    case EECALLBACK:
		sock->Blocking = 1;
		break;
	    case EEWOULDBLOCK: 
		call_out( (: eventServerWriteCallback :), 0, fd);
		return;
	    case EEALREADY:
		sock->Blocking = 1;
		return;
	    default:
		eventClose(sock);
		eventSocketError("Error in socket_write().", x);
		return;
	}
	if( sizeof(sock->Buffer) == 1 ) {
	    sock->Buffer = 0;
	    if( sock->Closing && !sock->Blocking ) {
		eventClose(sock);
	    }
	}
	else {
	    sock->Buffer = sock->Buffer[1..];
	}
    }
}

static void eventSocketError(string msg, int code) {
    log_file("servers", "Error code: " + code + "\n" + msg + "\n");
}

varargs int eventWrite(object owner, mixed val, int close) {
    class server sock;
    int fd = owner->GetDescriptor();

    if( Listen && Listen->Descriptor == fd ) {
	sock = Listen;
    }
    else if( Sockets[fd] ) {
	sock = Sockets[fd];
    }
    else {
	return 0;
    }
    if( owner != sock->Owner ) {
	return 0;
    }
    if( SocketType != STREAM || stringp(val)) {
	if( sock->Buffer ) {
	    sock->Buffer += ({ val });
	}
	else {
	    sock->Buffer = ({ val });
	}
    }
    else {
	buffer data = val;
	int size = sizeof(data);
	int count = (size/MaxBytes) + 1;
	
	if( !sock->Buffer ) {
	    sock->Buffer = ({});
	}
	for(int i=0; i<count; i++) {
	    int length, ptr;
	    buffer b;
	    
	    ptr = count * MaxBytes;
	    if( size - ptr > MaxBytes ) {
		length = MaxBytes;
	    }
	    else {
		length = size - ptr;
	    }
	    b = read_buffer(data, ptr, length);
	    sock->Buffer = ({ sock->Buffer..., b });
	}
    }

    sock->Closing = close;
    if( !sock->Blocking ) {
	eventServerWriteCallback(sock->Descriptor);
    }
    return 1;
}

/* ******************** server.c driver applies ******************* */
varargs static void create(int port, int type, string socket_obj) {
    daemon::create();
    SetNoClean(1);

    if( socket_obj ) {
	SocketObject = socket_obj;
    }
    if( !undefinedp(type) ) {
	SocketType = type;
    }
    if( !undefinedp(port) ) {
	Port = port;
	call_out((: eventCreateSocket :), 1, Port);
    }
}

