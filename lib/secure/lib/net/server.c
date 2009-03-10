/*    /lib/net/server.c
 *    From the Dead Souls LPC Library
 *    a TCP server object
 *    created by Descartes of Borg 950429
 *    Version: @(#) server.c 1.3@(#)
 *    Last modified: 96/12/19
 */

#include <lib.h>
#include <daemons.h>
#include NETWORK_H
#include <runtime_config.h>
#include <message_class.h>

inherit LIB_DAEMON;
string mcolor;
int mclass;
int ftp_port = PORT_FTP;

mapping ServerMap = ([]);
mapping Listen = ([]);

private static int          DestructOnClose= 0;
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
    mapping s; 

    trr("LIB_SERVER: eventClose trying to close: "+identify(sock),mcolor,mclass);
    if(mapp(sock)) {
        s = copy(sock);
    }
    if( intp(sock) ) {
        s = copy(Sockets[sock]);
    }
    else if( objectp(sock) ) {
        s = copy(Sockets[sock->GetDescriptor()]);
    }
    if( !s ) {
        return 0;
    }
    if( s["Blocking"] ) {
        s["Closing"] = 1;
        return 1;
    }
    if( Sockets[s["Descriptor"]] ) {
        map_delete(Sockets, s["Descriptor"]);
    }
    socket_close(s["Descriptor"]);
    if( s["Owner"] ) {
        s["Owner"]->eventSocketClosed();
    }
    if( DestructOnClose && sock == Listen ) {
        Destruct();
    }
    sock = 0;
}

int eventCreateSocket(int port) {
    int x;
    Listen["Blocking"] = 0; /* servers are not blocking to start */
    x = socket_create(SocketType,
            "eventServerReadCallback", 
            "eventServerAbortCallback");
    if( x < 0 ) {
        eventSocketError("Error in socket_create().", x);
        return x;
    }
    Listen["Descriptor"] = x;
    x = socket_bind(Listen["Descriptor"], port);
    if( x != EESUCCESS ) {
        eventClose(Listen);
        eventSocketError("Error in socket_bind().", x);
        return x;
    }
    x = socket_listen(Listen["Descriptor"], "eventServerListenCallback");
    if( x != EESUCCESS ) {
        eventClose(Listen);
        eventSocketError("Error in socket_listen().", x);
        return x;
    }
    trr("LIB_SERVER: eventCreateSocket, port: "+port+", x: "+x,mcolor,mclass);
}

static int Destruct() {
    if( daemon::Destruct() ) {
        foreach(int fd, mapping socket in Sockets) {
            trr("server:Destruct: fd: "+fd+", "+socket_address(fd),mcolor,mclass);
            if(socket && socket["Owner"])
                socket["Owner"]->eventShutdown();
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
    int fd = socket->GetDescriptor();
    if(!Sockets[fd]) Sockets[fd] = ([]);
    trr("LIB_SERVER: eventNewConnection, socket: "+identify(socket),mcolor,mclass);
    trr("LIB_SERVER: eventNewConnection, socket->GetDescriptor(): "+identify(socket->GetDescriptor()),mcolor,mclass);
    Sockets[fd]["Descriptor"] = socket->GetDescriptor();
    Sockets[fd]["Blocking"] = 0;
    Sockets[fd]["Owner"] = socket;
    socket->StartService(); // added for welcome
}

static void eventServerAbortCallback(int fd) {
    trr("server:eventServerAbortCallback: fd: "+fd+", "+socket_address(fd),mcolor,mclass);
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

    trr("server:eventServerListenCallback: fd: "+fd+", "+socket_address(fd),mcolor,mclass);
    x = socket_accept(fd,
            "eventServerReadCallback", 
            "eventServerWriteCallback");
    if( x < 0 ) {
        trr("Error in socket_accept().",mcolor,mclass);
        eventSocketError("Error in socket_accept().", x);
        return;
    }
    if( Sockets[x] ) {
        eventClose(Sockets[x]);
    }
    trr("LIB_SERVER SocketObject: "+SocketObject,mcolor,mclass);
    eventNewConnection(new(SocketObject, x, this_object()));
}

static void eventServerReadCallback(int fd, mixed val) {
    trr("server:eventServerReadCallback: fd: "+fd+", "+socket_address(fd),mcolor,mclass);
    trr("server: I think that Sockets["+fd+"] is: "+identify(Sockets[fd]),mcolor,mclass);
    if( !Sockets[fd] || !Sockets[fd]["Owner"] ) {
        trr("server: No owner found for this data.",mcolor,mclass);
        eventClose(fd);
        return;
    }
    else {
        trr("Sockets["+fd+"]: "+identify(Sockets[fd]),mcolor,mclass);
        trr("sizeof(val): "+sizeof(val),mcolor,mclass);
        trr("typeof(val): "+typeof(val),mcolor,mclass);
        if(bufferp(val)) trr("  val: "+identify(read_buffer(val)),mcolor,mclass);
        else trr("  val: "+identify(val),mcolor,mclass);
        Sockets[fd]["Owner"]->eventRead(val);
    }
}

static void eventServerWriteCallback(int fd) {
    int x;
    mapping sock;

    trr("server:eventServerWriteCallback: fd: "+fd+", "+socket_address(fd),mcolor,mclass);
    if( Listen && Listen["Descriptor"] == fd ) {
        sock = Listen;
    }
    else if( Sockets[fd] ) {
        sock = Sockets[fd];
    }
    else {
        return;
    }
    sock["Blocking"] = 0;
    if( !sock["Buffer"] && sock["Closing"] ) {
        eventClose(sock);
        return;
    }
    x = EESUCCESS;
    while( sock["Buffer"] && x == EESUCCESS ) {
        switch( x = socket_write(sock["Descriptor"], sock["Buffer"][0]) ) {
            case EESUCCESS:
                break;
            case EECALLBACK:
                sock["Blocking"] = 1;
                break;
            case EEWOULDBLOCK: 
                call_out( (: eventServerWriteCallback :), 0, fd);
                return;
            case EEALREADY:
                sock["Blocking"] = 1;
                return;
            default:
                eventClose(sock);
                eventSocketError("Error in socket_write().", x);
                return;
        }
        if( sizeof(sock["Buffer"]) == 1 ) {
            sock["Buffer"] = 0;
            if( sock["Closing"] && !sock["Blocking"] ) {
                eventClose(sock);
            }
        }
        else {
            sock["Buffer"] = sock["Buffer"][1..];
        }
    }
}

static void eventSocketError(string msg, int code) {
    log_file("servers", "Error code: " + code + "\n" + msg + "\n");
    trr("LIB_SERVER Error code: " + code + "\n" + msg + "\n","red",mclass);
}

varargs int eventWrite(object owner, mixed val, int close) {
    int fd = owner->GetDescriptor();
    mapping sock;

    trr("server:eventWrite: fd: "+fd+", "+socket_address(fd),mcolor,mclass);
    if(bufferp(val)) trr("       eventWrite: owner: "+identify(owner)+", val: "+identify(read_buffer(val)),mcolor,mclass);
    else trr("       eventWrite: owner: "+identify(owner)+", val: "+identify(val),mcolor,mclass);
    trr("       eventWrite: close: "+close,mcolor,mclass);

    if( Listen && Listen["Descriptor"] == fd ) {
        sock = Listen;
    }
    else if( Sockets[fd] ) {
        sock = Sockets[fd];
    }
    else {
        return 0;
    }
    if( owner != sock["Owner"] ) {
        return 0;
    }
    if( SocketType != STREAM || stringp(val)) {
        if( sock["Buffer"] ) {
            sock["Buffer"] += ({ val });
        }
        else {
            sock["Buffer"] = ({ val });
        }
    }
    else {
        buffer data = val;
        int size = sizeof(data);
        int count = (size/MaxBytes) + 1;

        if( !sock["Buffer"] ) {
            sock["Buffer"] = ({});
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
            sock["Buffer"] = ({ sock["Buffer"]..., b });
        }
    }

    sock["Closing"] = close;
    if( !sock["Blocking"] ) {
        eventServerWriteCallback(sock["Descriptor"]);
    }
    return 1;
}

/* ******************** server.c driver applies ******************* */
varargs static void create(int port, int type, string socket_obj) {
    daemon::create();
    SetNoClean(1);

    if(port == PORT_FTP){mcolor="green";mclass=MSG_FTP;}
    if(port == PORT_HFTP){mcolor="white";mclass=MSG_HFTP;}
    else if(port == PORT_HTTP){mcolor="cyan";mclass=MSG_HTTP;}
    else if(port == PORT_RCP){mcolor="yellow";mclass=MSG_RCP;}
    else if(port == PORT_OOB){mcolor="red";mclass=MSG_OOB;}
    else { mcolor="blue",mclass=MSG_CONV;}

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
