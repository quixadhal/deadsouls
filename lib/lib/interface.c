/*    /lib/interface.c
 *    from the Dead Soulsr1 Object Library
 *    handles user interface issues
 *    created by Descartes of Borg 950428
 *    Version: @(#) interface.c 1.4@(#)
 *    Last Modified: 96/10/15
 */

#include <lib.h>
#include <daemons.h>
#include <message_class.h>
#include "include/interface.h"

inherit LIB_CHAT;
inherit LIB_COMMAND;
inherit LIB_EDITOR;
inherit LIB_NMSH;

private string Terminal;
private mapping Blocked;
private int *Screen;
private static int LogHarass, Client;
private static mapping TermInfo;

static void create() {
    chat::create();
    command::create();
    editor::create();
    nmsh::create();
    Terminal = "unknown";
    Screen = ({ 80, 24 });
    Blocked = ([]);
}

static string process_input(string str) {
    SetCommandFail(0);
    if( Client ) {
        int cl;

	sscanf(str, "%d %s", cl, str);
    }
    if( (str = editor::process_input(str)) == "" ) return "";
    else {
	str = nmsh::process_input(str);
	if( str != "" ) {
	    return chat_command(str);
	}
	else {
	    return "";
	}
    }
}

static void terminal_type(string str) {
    if( !stringp(str) ) return;
    else SetTerminal(lower_case(str));
}

static void window_size(int width, int height) { SetScreen(width, height); }

void receive_message(string msg_class, string msg) {
    int cl = 0;
    
    if( msg_class[0] == 'N' ) {
	msg_class = msg_class[1..];
	cl |= MSG_NOWRAP;
    }
    else if( msg_class == "prompt" && msg_class == "editor" ) cl |= MSG_NOWRAP;
    switch(msg_class) {
        case "smell": case "sound": case "touch": 
	cl |= MSG_ENV;
	break;

	case "snoop":
	cl |= MSG_SYSTEM | MSG_NOCOLOUR;
	case "broadcast":
	cl |= MSG_SYSTEM;
	break;

	case "editor":
	cl |= MSG_EDIT;
	break;
	
	case "tell": case "shout":
	cl |= MSG_CONV;
        break;
	
        case "come": case "leave": case "telout": case "telin":
	cl |= MSG_ENV;
	break;

        case "living_item": case "inanimate_item":
	cl |= MSG_ROOMDESC;
	break;
	
        case "system": case "more":
	cl |= MSG_SYSTEM;
	break;

	case "prompt":
	cl = MSG_PROMPT;
	break;

	case "error":
	cl |= MSG_ERROR;
	break;

	case "help":
	cl |= MSG_HELP;

	default:
	cl |= MSG_ENV;
	
    }
    eventPrint(msg, cl);
}

static void receive_snoop(string str) { receive_message("snoop", "%"+str); } 

int Setup() {
    command::Setup();
    nmsh::Setup();
    TermInfo = (mapping)TERMINAL_D->query_term_info(Terminal);
}

varargs int eventPrint(string msg, mixed arg2, mixed arg3) {
    int msg_class;

    if( !msg ) return 0;
   if( !arg2 && !arg3 ) msg_class = MSG_ENV;
   else if( !arg2 ) {
	if( !intp(arg3) ) msg_class = MSG_ENV;
	else msg_class = arg3;
    }
    else if( !intp(arg2) ) msg_class = MSG_ENV;
    else msg_class = arg2;
    if( !(msg_class & MSG_NOBLOCK) && GetBlocked("all") ) return 0;
    if( GetLogHarass() )
      log_file("harass/" + GetKeyName(), strip_colours(msg) + "\n");
    if( !TermInfo )
      TermInfo = (mapping)TERMINAL_D->query_term_info(GetTerminal());
    if( !(msg_class & MSG_NOCOLOUR) ) {
	int indent;

	if( msg_class & MSG_CONV ) indent = 4;
	else indent = 0;
	if( msg_class & MSG_NOWRAP )
	    msg = terminal_colour(msg + "%^RESET%^", TermInfo);
	else
	    msg = terminal_colour(msg + "%^RESET%^\n", TermInfo,
				  GetScreen()[0], indent);
    }
    else if( !(msg_class & MSG_NOWRAP) ) msg = wrap(msg, GetScreen()[0]-1);
    if( Client ) receive("<" + msg_class + " " + msg + " " + msg_class +">\n");
    else receive(msg);
    return 1;
}
	  
varargs int SetBlocked(string type, int flag) {
    if( !type ) return 0;
    if( !flag ) flag = !Blocked[type];
    if( Blocked[type] == 2 && !archp(this_player()) ) {
        this_player()->eventPrint("Unable to unblock " + type + ".");
        return -1;
    }
    Blocked[type] = flag;
    message("system", "You are "+(Blocked[type] ? "now blocking" :
      "no longer blocking")+" "+type+".", this_object());
    return Blocked[type];
}

int GetBlocked(string type) { return (Blocked["all"] || Blocked[type]); }

int SetClient(int x) {
    return 0;
    if( x ) SetTerminal("unknown");
    return (Client = x);
}

int GetClient() { return Client; }

int SetLogHarass(int x) {
    string txt;

    if( GetForced() || (this_player(1) != this_object()) ) return LogHarass;
    if( LogHarass == x ) return LogHarass;
    if( x ) {
        txt = "**************** Start of Log *****************\n"+
            "Time: " + ctime( time() ) + "\n";
        if( environment( this_object() ) ) txt += "Place: " +
            file_name( environment( this_object() ) ) + "\n";
    } else {
        txt = "**************** End of Log *****************\n"+
            "Time: " + ctime( time() ) + "\n";
    }
    log_file("harass/" + GetKeyName(), txt);
    return (LogHarass = x);
}

int GetLogHarass() { return LogHarass; }

int *SetScreen(int width, int height) { 
    if( !width || !height ) return Screen;
    width--;
    if( width * height > __LARGEST_PRINTABLE_STRING__ ) {
	if( width > height ) width = __LARGEST_PRINTABLE_STRING__/height;
	else if( height > width ) height = __LARGEST_PRINTABLE_STRING__/width;
	else width = height = (__LARGEST_PRINTABLE_STRING__-1)/2;
    }
    return (Screen = ({ width, height })); 
}

int *GetScreen() { return Screen; }

string SetTerminal(string terminal) { 
    switch( terminal ) {
        case "iris-ansi-net": case "vt100": case "vt220": case "vt102":
        case "vt300": case "dec-vt100":
	    terminal = "ansi";
	    break;
	case "unknown": case "ansi": case "freedom": case "ansi-status":
	case "xterm": 
	    break;
	case "console": case "ibm-3278-2":
	    terminal = "unknown";
	    break;
	default:
	    log_file("terminals", "Unknown terminal type: " + terminal + "\n");
	    terminal = Terminal;
	    break;
    }
    if( terminal != Terminal ) 
      TermInfo = (mapping)TERMINAL_D->query_term_info(terminal);
    return Terminal = terminal;
}

string GetTerminal() { return Terminal; }

string GetKeyName() { return 0; }

