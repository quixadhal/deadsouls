#include <lib.h>
#include NETWORK_H
inherit LIB_ITEM;

int attempting, connected, socket ;
object person, player;
string preset;
string DS_IP = "66.197.134.110 8000";

static void create()
{
    string myname = mud_name();
    item::create();
    SetKeyName("telnet_room_client");
    SetShort( "a telnet client" ) ;
    SetId(({"client","telnet client"}));
    SetLong( "It's a small pocket sized telnet terminal.\n"
            "It appears customized to connect to Dead Souls MUD only.\n"
            "Use 'telnet' or 'connect' to begin.\n\nCommands:\n"
            "[connect|telnet] : start telnet session.\nreset [client]"
            " : reset the telnet client.\nreconnect : reconnect to session"
            " (if you go netdead)\n");
    SetMass(0) ;
    attempting = 0 ;
    connected = 0 ;
    socket = 0 ;
    person = 0 ;
    set_heart_beat(2);
    if(!strsrch(myname, "Dead Souls") && myname != "Dead Souls Prime"){
        DS_IP = "66.197.134.110 6666";
    }
}

void heart_beat(){
    if(!this_object() || !clonep(this_object())) return;
    if(!environment() || !living(environment()) 
            || !environment(environment())) 
        eventDestruct();
    if(this_object() && environment() && environment(environment())){
        if(!connected && base_name(environment(environment())) 
                != "/domains/default/room/telnet_room")
            eventDestruct();
    }
}

void init()
{  
    add_action( "do_connect", ({ "connect", "telnet" }) ) ;
    add_action( "do_reset", "reset" ) ;
    add_action( "do_reconnect", "reconnect" ) ;
}

void SetConnection(string str){
    if(str) preset = str;
}

int do_reconnect()
{
    if( !connected )
    {
        notify_fail( "The telnet client is not connected!\n" ) ;
        return 0 ;
    }
    write("Reconnecting into telnet session.\n");
    person = this_player() ;
    input_to( "parse_comm", 0 ) ;
    return 1 ;
}

int do_reset( string args )
{
    notify_fail( "Usage: reset client\n" ) ;
    if( !args || args == "" )
    {
        return 0 ;
    }
    if( args != "client" )
    {
        return 0 ;
    }
    write("Resetting telnet client ...\n");
    if( connected )
    {
        if( socket )
        {
            socket_close( socket ) ;
        }
    }
    attempting = 0 ;
    connected = 0 ;
    socket = 0 ;
    person = 0 ;
    write("Done!\n");
    return 1 ;
}

string help()
{
    return "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n"
        "  Usage : connect [ip_address] [port]\n"
        "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n"
        "Note: use telnet port number 23 if you \n"
        "      are connecting to a normal site. \n"
        "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n" ;
}

varargs int do_connect(string args, object whom)
{
    int new_socket, sc_result, port ;
    string error, ip_address ;

    if(preset) args = preset;
    else args = DS_IP;

    if(args != DS_IP){
        if(!this_player()) return 1;
        if(!telnet_privp(this_player())){
            this_player()->eventPrint("You aren't a member of the group of users permitted "
                    "to use this mud's telnet facility.");
            return 1;
        }
    }

    if( !args || args == "" )
    {
        notify_fail( help() ) ;
        return 0 ;
    }
    if( sscanf( args, "%s %d", ip_address, port ) != 2 )
    {
        notify_fail( help() ) ;
        return 0 ;
    }
    if( attempting )
    {
        notify_fail( "Telnet connection attempt already in progress.\n" ) ;
        return 0 ;
    }
    if( connected )
    {
        notify_fail( "Already connected...\n" ) ;
        return 0 ;
    }
    new_socket = socket_create( STREAM, "read_callback", "close_callback" ) ;
    if( new_socket < 0 )
    {
        switch( new_socket )
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
                error = "Unknown error code: " + new_socket + ".\n" ;
                break ;
        }
        notify_fail( "Unable to connect, problem with socket_create.\n"
                "Reason: " + error ) ;
        return 0 ;
    }
    sc_result = socket_connect( new_socket, ip_address + " " + port,
            "read_callback", "write_callback" ) ;
    if( sc_result != EESUCCESS )
    {
        notify_fail( "Failed to connect.\n" ) ;
        return 0 ;
    }
    attempting = 1 ;
    socket = new_socket ;
    person = (object)previous_object() ;
    if(!whom) player=this_player();
    else player = whom;
    input_to( "parse_comm", 0 ) ;
    return 1 ;
}

void read_callback( int fd, mixed message )
{
    player->eventPrint(message);
}

void close_callback( int fd )
{
    if( connected )
    {
        write("Connection closed by foreign host.\n");
    }
    if( attempting )
    {       
        write("Attempt failed.\n");
    }
    write("Type 'dcon' to finalize exit.\n");
    socket_close( fd ) ;
    attempting = 0 ;
    connected = 0 ;
    socket = 0 ;
}

void write_callback( int fd )
{
    attempting = 0 ;
    connected = 1 ;
}

int parse_comm( string str )
{
    if(str=="dcon" || str=="quit")
    {
        write("You return from your visit to another mud!\n");
        socket_close( socket ) ;
        attempting = 0 ;
        connected = 0 ;
        socket = 0 ;
        person = 0 ;
        this_player()->eventDescribeEnvironment();
        return 1 ;
    } else {
        if( !connected )
        {
            write("You are not connected. Type 'dcon' to exit.\n");
            input_to( "parse_comm", 0 ) ;
            return 1 ;
        }
        if( attempting )
        {
            write("Please wait, still attempting connection, "
                    "type 'dcon' to exit.\n");
            input_to( "parse_comm", 0 ) ;
            return 1 ;
        }
        socket_write( socket, str + "\n" ) ;
        input_to( "parse_comm", 0 ) ;
        return 1 ;
    }
}
