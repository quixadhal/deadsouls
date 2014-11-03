/*	/secure/cmds/creators/dsversion.c
 *  Created by Brodbane of Eve
 *	Date Created: 4/1/06
 */

#include <lib.h>
#include NETWORK_H
#include <socket_err.h>
#include <message_class.h>

inherit LIB_DAEMON;

#define HTTP_ADDRESS "97.107.133.86"
#define HTTP_HOST "dead-souls.net"
#define HTTP_PORT 80
#define HTTP_PATH "/RELEASE_NOTES"
#define NOTES_DELIM "----"
#undef _DEBUG

#define SAVE_FILE "/doc/RELEASE_NOTES_HTTP"


#define SOCK_FIND 0
#define SOCK_CONNECTING 1
#define SOCK_CONNECTED 2
#define SOCK_DISCONNECTED 3
#define SOC_ERROR 4

int socket;
int status;
object player;
object person;

string results, s;

void sendHTTPGet();

int http_file_found;

private string args_list;

mixed ProcessHTTPResult(){
    string * parts;
    string * arg_array;
    string * temp;
    parts = explode( results, NOTES_DELIM )[1..];
    if(!sizeof(parts)) return 0;
    temp = explode( parts[0], "---" );
    player->eventPrint( "Current Version of "+mud_name()+": " + mudlib_version() );
    player->eventPrint( "Latest Version of Dead Souls: %^RED%^"+trim(temp[0])+ "%^RESET%^" );
    write_file( SAVE_FILE, results, 1 );

    arg_array = explode( args_list, " " );
    foreach( string str in arg_array ){
        temp = regexp( parts, args_list+" +" );
        if( sizeof( temp ) > 1 ) temp = regexp( parts, args_list+" " );
        if( sizeof( temp ) > 3 ){
            player->eventPrint( "Too many results, truncating..." );
            return print_long_string(player, temp[0]+temp[1]+temp[2],1 );
        }
        else{
            s = implode(temp,"\n%^CYAN%^----%^RESET%^ ");
            write_file( SAVE_FILE, results, 1 );
            s += "\nFor complete release notes refer to "+SAVE_FILE;
            print_long_string(player, "%^CYAN%^----%^RESET%^ "+s,1 );
        }
    }
    return 1;
}

void read_callback( int fd, mixed message ){
    results += message;
}

void write_callback( int fd ){
#ifdef _DEBUG
    player->eventPrint("Connected!");
#endif
    status = SOCK_CONNECTED;
    http_file_found = 0;
    sendHTTPGet();
}

void close_callback( int fd ){
    if( status == SOCK_CONNECTED )
    {
        // Process HTML here
#ifdef _DEBUG
        player->eventPrint("dsversion: Connection closed by host.");
#endif
        ProcessHTTPResult();
    }
    if( status == SOCK_CONNECTING )
    {       
        player->eventPrint("dsversion: Connection attempt failed.");
    }
    socket_close( fd ) ;
    status = SOCK_DISCONNECTED;
}

void sendHTTPGet()
{
    string str ="GET "+HTTP_PATH+" HTTP/1.0"+CARRIAGE_RETURN+"\n"+
        "Host: "+HTTP_HOST+CARRIAGE_RETURN"\n" +
        "User-Agent: "+ player->GetName() + "@" + mud_name() + " " +
        mudlib()+ "/" + mudlib_version() +" ("+ query_os_type()+";) "+ 
        version() + CARRIAGE_RETURN+"\n"+CARRIAGE_RETURN+"\n";
    int result = 0;
    results = "";
#ifdef _DEBUG
    player->eventPrint("Sending HTTP get request..." );
#endif
    if( status == SOCK_CONNECTED )
    {
        result = socket_write( socket, str );
#ifdef _DEBUG
        player->eventPrint( "HTTP request sent to " + 
                identify(socket_status(socket)) + " result = "+result );
#endif
    }
}

int openHTTPConnection()
{

    int sock, sc_result;
    string error;
    status = SOCK_FIND;


    status = SOCK_CONNECTING;
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
    write("Attempting to connect to "+HTTP_HOST+ " on port "+ HTTP_PORT + "\n");
#endif	
    sc_result = socket_connect( sock, HTTP_ADDRESS + " " + HTTP_PORT,
            "read_callback", "write_callback" ) ;
    if( sc_result != EESUCCESS )
    {
        status = SOCK_DISCONNECTED;
        notify_fail( "Failed to connect.\n" ) ;
        return 0 ;
    }
    else{
#ifdef _DEBUG
        player->eventPrint( "Socket connecting..." );
#endif
    }

    socket = sock;
    return 1;
}

void hostResolved( string address, string resolved, int key )
{
    if( !resolved ){
#ifdef _DEBUG
        player->eventPrint( "Unable to resolve "+address );
#endif
        return;
    }
#ifdef _DEBUG
    player->eventPrint( "Resolved "+address+" to "+ resolved );
#endif
    player->eventPrint( "Retrieving updated release notes from: http://www.dead-souls.net\n" );
    openHTTPConnection();
}

void resolveHost()
{
    resolve( HTTP_HOST, "hostResolved" );
}


mixed cmd(string args) {
    player = previous_object();
    if(args) args_list = args;
    else args_list = "";
    openHTTPConnection();
    return 1;
}

string GetErorMessage() {
    return "There was a problem";
}

string GetHelp() {
    return ("Syntax: dsversion [version]\n\n" +
            "Shows the latest version of Dead Souls and release notes.\n"+
            "e.g. dsversion, dsversion 2.9a12, dsversion 2.0r1");
}
