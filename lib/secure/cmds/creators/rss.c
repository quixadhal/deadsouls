#include <lib.h>
#include NETWORK_H
#include <socket_err.h>
#include <message_class.h>

inherit LIB_DAEMON;

#define HTTP_ADDRESS "204.209.44.12"
#define HTTP_HOST "lpmuds.net"
#define HTTP_PORT 80
#define HTTP_PATH "/forum/index.php?type=rss;action=.xml"
#define NOTES_DELIM "----"

#define SAVE_FILE "/doc/RELEASE_NOTES_HTTP"


#define SOCK_FIND 0
#define SOCK_CONNECTING 1
#define SOCK_CONNECTED 2
#define SOCK_DISCONNECTED 3
#define SOC_ERROR 4

#define _DEBUG

int socket;
int status;
object player;
object person;
mapping Feeds = ([]);
mapping Sockets = ([]);

string results, s;

void sendHTTPGet();
void Setup();

int http_file_found;

private string args_list;

void create(){
    daemon::create();
    if(!Feeds) Feeds = ([]);
    if(!Sockets) Sockets = ([]);
    call_out((: Setup :), 2);
}

void add_feed(string name, mapping feed){
    Feeds[name] = feed;
}

void Setup(){
    set_heart_beat(1);

    add_feed("lpmuds-forum", ([
                "name": "LPMuds.net",
                "ip_port": "204.209.44.12 80",
                "host": "lpmuds.net",
                "req": "GET /forum/index.php?type=rss;action=.xml HTTP/1.0",
                "wait": 15,
                "min_wait": 2,
                "max_wait": 15,
                "timer": -1,
                "userid": "LPMuds",
                ]) );

    add_feed("tmc-forums", ([
                "name": "Mudconnect.com Forums",
                "ip_port": "66.150.96.119 80",
                "host": "feeds.feedburner.com",
                "req": "GET /TMCForums HTTP/1.0",
                "wait": 15,
                "min_wait": 2,
                "max_wait": 20,
                "timer": -1,
                "userid": "TMC",
                ]) );
    add_feed("tmc-mudconnect", ([
                "name": "Mudconnect.com",
                "ip_port": "66.150.96.119 80",
                "host": "feeds.feedburner.com",
                "req": "GET /Mudconnect HTTP/1.0",
                "wait": 15,
                "min_wait": 2,
                "max_wait": 30,
                "timer": -1,
                "userid": "TMC",
                ]) );

    add_feed("mudbytes-forum", ([
                "name": "MudBytes",
                "ip_port": "209.190.9.170 80",
                "host": "www.mudbytes.net",
                "req": "GET /index.php?a=rssfeed HTTP/1.0",
                "wait": 15,
                "min_wait": 2,
                "max_wait": 15,
                "timer": -1,
                "userid": "MudBytes",
                ]) );

    add_feed("smaugmuds-forum", ([
                "name": "SmaugMuds.org",
                "ip_port": "66.218.49.113 80",
                "host": "www.smaugmuds.org",
                "req": "GET /index.php?a=rssfeed HTTP/1.0",
                "wait": 15,
                "min_wait": 2,
                "max_wait": 15,
                "timer": -1,
                "userid": "SmaugMuds",
                ]) );

    add_feed("topmudsites-forum", ([
                "name": "Top Mud Sites Forum",
                "ip_port": "72.232.233.194 80",
                "host": "www.topmudsites.com",
                "req": "GET /forums/external.php?type=RSS2&lastpost=true HTTP/1.0",
                "append": ([ "ext": ".html", "string": "-new-post" ]),
                "wait": 15,
                "min_wait": 2,
                "max_wait": 15,
                "timer": -1,
                "userid": "TMS",
                ]) );

    add_feed("arthmoor-forum", ([
                "name": "Arthmoor Internet Services: MUD Hosting",
                "ip_port": "66.218.49.113 80",
                "host": "www.arthmoor.com",
                "req": "GET /forum/index.php?a=rssfeed HTTP/1.0",
                "wait": 15,
                "min_wait": 2,
                "max_wait": 3 * 60,
                "timer": -1,
                "userid": "Arthmoor",
                ]) );

    add_feed("mudmagic-lpc", ([
                "name": "Mud Magic: LPC Lounge",
                "ip_port": "67.19.238.52 80",
                "host": "www.mudmagic.com",
                "req": "GET /rss/boards/26 HTTP/1.0",
                "wait": 15,
                "min_wait": 2,
                "max_wait": 3 * 60,
                "timer": -1,
                "userid": "MudMagic",
                ]) );
    add_feed("mudmagic-staff", ([
                "name": "Mud Magic: Staff Wanted",
                "ip_port": "67.19.238.52 80",
                "host": "www.mudmagic.com",
                "req": "GET /rss/boards/5 HTTP/1.0",
                "wait": 15,
                "min_wait": 2,
                "max_wait": 60,
                "timer": -1,
                "userid": "MudMagic",
                ]) );

    add_feed("metaplace-news", ([
                "name": "Metaplace.com Developer's Blog",
                "ip_port": "66.150.96.119 80",
                "host": "feeds.feedburner.com",
                "req": "GET /metaplace HTTP/1.0",
                "char-encoding": "utf8",
                "wait": 15,
                "min_wait": 2,
                "max_wait": 60,
                "timer": -1,
                "userid": "Metaplace",
                ]) );

    add_feed("theonion", ([
                "name": "The Onion",
                "ip_port": "66.150.96.119 80",
                "host": "feeds.theonion.com",
                "req": "GET /theonion/daily HTTP/1.0",
                "sortBy": "pubDate",
                "char-encoding": "utf8",
                "wait": 15,
                "min_wait": 2,
                "max_wait": 24 * 60,
                "timer": -1,
                "userid": "TheOnion",
                ]) );
}


mixed ProcessHTTPResult(int fd)
{
    string * parts;
    string * arg_array;
    string * temp;
    write_file("/tmp/"+Sockets[fd]["name"],Sockets[fd]["result"]);
    //parts = explode( results, NOTES_DELIM );
    return 1;
}

void read_callback( int fd, mixed message )
{
    Sockets[fd]["result"] += message;
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
        ProcessHTTPResult(fd);
    }
    if( status == SOCK_CONNECTING )
    {       
        player->eventPrint("Connection attempt failed.");
    }
    socket_close( fd ) ;
    map_delete(Sockets,fd);
    status = SOCK_DISCONNECTED;
}

void sendHTTPGet()
{
    string str =Feeds[args_list]["req"]+CARRIAGE_RETURN+"\n"+
        "Host: "+Feeds[args_list]["host"]+CARRIAGE_RETURN"\n" +
        "User-Agent: RSS_D@" + mud_name() + " " +
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
    write("Attempting to connect to "+HTTP_HOST+ " on port "+ HTTP_PORT + "\n");
    sc_result = socket_connect( sock, Feeds[args_list]["ip_port"],
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
    Sockets[socket] = ([ "inception" : time(), "name" : args_list, "result" : "" ]);
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
    if(args == "list"){
        write("Known feeds: "+implode(keys(Feeds),", "));
        return 1;
    }
    player = previous_object();
    if(args) args_list = args;
    else args_list = "lpmuds-forum";
    rm("/tmp/"+args_list);
    openHTTPConnection();
    return 1;
}

string GetErorMessage() {
    return "There was a problem";
}

string GetHelp(){
    return "Syntax: rss\n\nwhatever";
}
