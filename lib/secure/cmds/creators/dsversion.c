/*	/secure/cmds/creators/dsversion.c
 *  Created by Brodbane of Eve
 *	Date Created: 4/1/06
 */

#include <lib.h>
#include <network.h>
#include <socket_err.h>
#include <message_class.h>

inherit LIB_DAEMON;

#define HTTP_ADDRESS "149.152.218.102"
#define HTTP_HOST "www.dead-souls.net"
#define HTTP_PORT 80
#define HTTP_PATH "/RELEASE_NOTES"
#define NOTES_DELIM "----"

#define SAVE_FILE "/doc/RELEASE_NOTES_HTTP"


#define SOCK_FIND 0
#define SOCK_CONNECTING 1
#define SOCK_CONNECTED 2
#define SOCK_DISCONNECTED 3
#define SOC_ERROR 4

//#define _DEBUG

int socket;
int status;
object player;
object person;

string results;

void sendHTTPGet();

int http_file_found;

private string args_list;

void ProcessHTTPResult()
{
    int i = 0;
    string * parts;
    string * arg_array;
    string * temp;

    //player->eventPrint(results);
    parts = explode( results, NOTES_DELIM );
    //player->eventPrint( "Size of elements "+sizeof(parts) );

    //found = regexp( parts, args_list+" +" );
    //player->eventPrint( sizeof( found ) );
    //if( !args_list ){
    //player->eventPrint( parts[0] );
    temp = explode( parts[0], "---" );
    player->eventPrint( "Current Version of "+mud_name()+": " + mudlib_version() );
    player->eventPrint( "Latest Version of Dead Souls: %^RED%^"+trim(temp[0])+ "%^RESET%^" );
    write_file( SAVE_FILE, results, 1 );
    //player->eventPrint( "\nFor complete release notes refer to "+SAVE_FILE );
    //return;
    //}

    arg_array = explode( args_list, " " );
    foreach( string str in arg_array ){
	temp = regexp( parts, args_list+" +" );
	if( sizeof( temp ) > 3 ){
	    player->eventPrint( "Too many results, truncating..." );
	    player->eventPrint( temp[0]+temp[1]+temp[2] );
	}
	else{
	    foreach( string s in temp )
	    {
		player->eventPrint( "---- "+s );
		i++;
	    }
	}
    }


    write_file( SAVE_FILE, results, 1 );
    player->eventPrint( "For complete release notes refer to "+SAVE_FILE );
}

void read_callback( int fd, mixed message )
{
    if( !http_file_found ){
	if( message[9..11] != "200" ){
	    player->eventPrint( "Error, unable to locate page requested." );

	    http_file_found = 3;
	}
	else
	{
	    //player->eventPrint( message ); Uncomment this to see http header debug
	    http_file_found = 1;
	}
    }
    else if( http_file_found == 1 ){
	//player->eventPrint( message );
	results += message;
    }
    else{

    }
    //player->eventPrint( message );
    //results += message;
    //player->eventPrint( message );
    //player->eventPrint( "------------------" );
}

void write_callback( int fd )
{
#ifdef _DEBUG
    player->eventPrint("Connected!");
#endif
    status = SOCK_CONNECTED;
    http_file_found = 0;
    sendHTTPGet();
}

void close_callback( int fd )
{
    if( status == SOCK_CONNECTED )
    {
	// Process HTML here
#ifdef _DEBUG
	player->eventPrint("Connection closed by host.");
#endif
	ProcessHTTPResult();
    }
    if( status == SOCK_CONNECTING )
    {       
	player->eventPrint("Connection attempt failed.");
    }
    socket_close( fd ) ;
    status = SOCK_DISCONNECTED;
}

void sendHTTPGet()
{
    string str ="GET "+HTTP_PATH+" HTTP/1.0\r\n"+
    "Host: "+HTTP_HOST+"\r\n" +
    "User-Agent: "+ player->GetName() + "@" + mud_name() + " " +
    mudlib()+ "/" + mudlib_version() +" ("+ query_os_type()+";) "+ 
    version() + "\r\n\r\n";
    int result = 0;
    results = "";
#ifdef _DEBUG
    player->eventPrint("Sending HTTP get request..." );
#endif
    if( status == SOCK_CONNECTED )
    {
	result = socket_write( socket, (string)str );
#ifdef _DEBUG
	player->eventPrint( "HTTP request sent to " + socket + " result = "+result );
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
    //resolveHost();
    openHTTPConnection();
    return 1;
}

string GetErorMessage() {
    return "There was a problem";
}

string GetHelp() {
    return ("Syntax: dsversion [version]\n\n" +
      "Shows the latest version of Dead Souls and release notes.\n"+
      "e.g. dsversion, dsversion r1, dsversion 2.0r1");
}
















