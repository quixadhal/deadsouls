/* This daemon is not intended to be monolithic. It should
 * be cloned, with each cloned version handling a single
 * connection and transfer.
 */

#include <lib.h>
#include NETWORK_H
#include <socket_err.h>
#include <message_class.h>
#include <daemons.h>
#define _DEBUG

inherit LIB_DAEMON;

int socket;
int status;
object Owner;
mapping PendingResolves = ([]);

string path, host, address, port, results, s;

void sendHTTPGet();

int http_file_found;

private string args_list;

varargs void create(object owner){
    daemon::create();
    Owner = owner;
    if(!PendingResolves) PendingResolves = ([]);
}

mixed ProcessHTTPResult()
{
    if(Owner){
        Owner->eventReceiveWebData(results,path);
    }
    return 1;
}

void read_callback( int fd, mixed message )
{
    results += message;
}

void write_callback( int fd )
{
    http_file_found = 0;
    sendHTTPGet();
}

void close_callback( int fd )
{
    ProcessHTTPResult();
    socket_close( fd ) ;
    ::eventDestruct();
}

void sendHTTPGet()
{
    string str ="GET "+path+" HTTP/1.0"+CARRIAGE_RETURN+"\n"+
        "Host: "+host+CARRIAGE_RETURN"\n" +
        "User-Agent: WGET_D@" + mud_name() + " " +
        mudlib()+ "/" + mudlib_version() +" ("+ query_os_type()+";) "+ 
        version() + CARRIAGE_RETURN+"\n"+CARRIAGE_RETURN+"\n";
    int result = 0;
    results = "";
    result = socket_write( socket, str );
}

int openHTTPConnection()
{

    int sock, sc_result;
    string error;

    sock = socket_create( STREAM, "read_callback", "close_callback" ) ;
    if (sock < 0) { 
        switch( sock )
        {
            case EEMODENOTSUPP :
                error = "Socket mode not supported.\n" ;
                break ;
            case EESOCKET :
                error = "Problem creating socket.\n" ;
                break ;
            case EESETSOCKOPT :
                error = "Problem with setsockopt.\n" ;
                break ;
            case EENONBLOCK :
                error = "Problem with setting non-blocking mode.\n" ;
                break ;
            case EENOSOCKS :
                error = "No more available efun sockets.\n" ;
                break ;
            case EESECURITY :
                error = "Security violation attempted.\n" ;
                break ;
            default :
                error = "Unknown error code: " + sock + ".\n" ;
                break ;
        }
        notify_fail( "Unable to connect, problem with socket_create.\n"
                "Reason: " + error ) ;
        return 0 ;
    }
#ifdef _DEBUG
    write("Attempting to connect to "+host+ " on port "+ port + "\n");
#endif	
    sc_result = socket_connect( sock, address + " " + port,
            "read_callback", "write_callback" ) ;
    if( sc_result != EESUCCESS )
    {
        notify_fail( "Failed to connect.\n" ) ;
        return 0 ;
    }
    else{
    }

    socket = sock;
    return 1;
}

void hostResolved( string address, string resolved, int key )
{
    if( !resolved ){
        return;
    }
    openHTTPConnection();
}

void resolveHost()
{
    resolve( host, "hostResolved" );
}


mixed eventGet(string args) {
    string foo, bar;
    int baz, key, a, b, c, d;

    if(!strsrch(args,"http://")){
        args = replace_string(args,"http://","",1);
    }
    address = first_string_element(args,"/");
    args = replace_string(args,address,"");
    if(sscanf(args, "%s -n %s", foo, bar) == 2){
        host = bar;
        args = foo;
    }
    else host = address;
    path = args;
    if(sscanf(address,"%s:%d",foo, baz) == 2){
        address = foo;
        port = ""+baz;
    }
    else port = "80";
    if(args) args_list = args;
    else args_list = "";
    if(sscanf(address,"%d.%d.%d.%d",a,b,c,d) != 4){
        if(!RESOLV_D->GetResolving()){
            return "Mud is not resolving. Try again with a numerical address.";
        }
        PendingResolves[key] = ([ "port" : port, "path" : path ]);
        key = RESOLV_D->eventResolve(address,"resolve_callback");
        return key;
    }
    openHTTPConnection();
    return 1;
}

void resolve_callback(string name, string ip, int key){
    string ret = ip+":"+port + path+" -n "+name;
    eventGet(ret);
}


string GetErorMessage() {
    return "There was a problem";
}

string GetHelp() {
    return ("Syntax: dsversion [version]\n\n" +
            "Shows the latest version of Dead Souls and release notes.\n"+
            "e.g. dsversion, dsversion r1, dsversion 2.0r1");
}

