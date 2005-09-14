#include <lib.h>
#include <network.h>
#include <socket_err.h>
inherit LIB_ITEM;
 
int attempting, connected, socket ;
object person ;
object player;
 
static void create()
{
   item::create();
   SetKeyName( "client" ) ;
   SetShort( "a telnet client" ) ;
   SetId(({ "telnet","terminal" })) ;
   SetLong( "It's a small pocket sized telnet terminal.\n"
            "Use 'telnet' or 'connect' to begin.\n\nCommands:\n"
            "[connect|telnet] : start telnet session.\nreset [client]"
            " : reset the telnet client.\nreconnect : reconnect to session"
            " (if you go netdead)\n");
   SetMass( 5 ) ;
   attempting = 0 ;
   connected = 0 ;
   socket = 0 ;
   person = 0 ;
}
 
void init()
{  
   add_action( "do_connect", ({ "connect", "telnet" }) ) ;
   add_action( "do_reset", "reset" ) ;
   add_action( "do_reconnect", "reconnect" ) ;
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
 
int do_connect(string args)
{
   int new_socket, sc_result, port ;
   string error, ip_address ;
 
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
   player=this_player();
   write("Telnet Client Version 1.2\n");
   write("Attempting connection to: " + ip_address + " " + port + "...\nEnter 'dcon' at any time to abort.\n");
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
   write("Connected...\n");
   attempting = 0 ;
   connected = 1 ;
}
 
int parse_comm( string str )
{
   if( str == "dcon" )
    {
       write("Disconnecting...\n");
       socket_close( socket ) ;
       attempting = 0 ;
       connected = 0 ;
       socket = 0 ;
       person = 0 ;
       return 1 ;
    } else {
       if( !connected )
        {
           write("You are not connected. Type \"dcon\" to stop using the client.\n");
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
 
