/*    /lib/interface.c
 *    from the Dead Souls Object Library
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
inherit LIB_CHARIO;
inherit LIB_NMSH;
inherit LIB_CEDIT;

private string Terminal;
private mapping Blocked;
private int *Screen;
private static int LogHarass, Client;
private static mapping TermInfo;
string MessageQueue;
int PauseMessages, annoyblock;
int MessageExceptions, BlockAnnoying;

static void create(){
    chat::create();
    command::create();
    editor::create();
    nmsh::create();
    Terminal = "ansi";
    Screen = ({ 80, 20 });
    Blocked = ([]);
}

static string process_input(string str){
    command::SetCommandFail(0);
    str = command::process_input(str);
    if( Client ){
        int cl;
        sscanf(str, "%d %s", cl, str);
    }
    if( (str = editor::process_input(str)) == "" ){
        return "";
    }
    else {
        str = nmsh::process_input(str);
        if( str != "" ){
            return chat_command(str);
        }
        else {
            return "";
        }
    }
}

static void terminal_type(string str){
    if( !stringp(str) ) return;
    else SetTerminal(lower_case(str));
}

static void window_size(int width, int height){ 
    if(query_verb() == "screen" || 
            !this_object()->GetProperty("screenlock")){
        SetScreen(width, height); 
    }
}

varargs void eventReceive(string message, int noprompt, int noerase){
    int max_length = __LARGEST_PRINTABLE_STRING__ - 192;
    string prompt = this_object()->GetPrompt(!(in_edit(this_object())));
    string *stack = call_stack(2);
    if(stack[1] == "write2" && message != "\n"){
        noerase = 1;
    }
    if(sizeof(message) > max_length){
        while(sizeof(message)){
            string tmp = message[0..max_length];
            receive(tmp+(noprompt ? "" : prompt));
            message = replace_string(message, tmp, "");
        }
    }
    else {
        if(!noerase && this_object()->GetProperty("reprompt")){
            this_object()->erase_prompt();
        }
        receive(message);
        
        //This check is so that players whose charmode is temporarily
        //suspended (so that they can be in the pager) don't have their
        //charmode prematurely re-enabled.
        if(!in_pager()) this_object()->CheckCharmode();

        //This check is so that players in charmode who currently are
        //typing a big line don't have their last-received-message
        //overwritten by the redraw of their screen on the next
        //keypress. I'm intentionally not adding a was_charmode check
        //because at the moment that seems like overkill and like it
        //might be annoying.
        if(query_charmode()){
            int buff = sizeof(this_object()->GetCharbuffer());
            if(buff){
                int scrn = this_object()->GetScreen()[0];
                int prmpt = sizeof(strip_colours(this_object()->GetPrompt()));
                if((prmpt + buff) >= scrn){
                    receive("\n");
                    this_object()->RedrawPrompt();
                }
            }
        }
    }
}

void receive_message(mixed msg_class, string msg){
    int cl = 0;
    if(intp(msg_class)){
        cl = msg_class;
        eventPrint(msg, cl);
        return;
    }

    else if( msg_class[0] == 'N' ){
        msg_class = msg_class[1..];
        cl |= MSG_NOWRAP;
    }
    else if( msg_class == "prompt" && msg_class == "editor" ) cl |= MSG_NOWRAP;

    switch(msg_class){
        case "smell": case "sound": case "touch": 
            cl |= MSG_ENV;
        break;

        case "receive":
            cl |= MSG_RECEIVE;
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

static void receive_snoop(string str){ receive_message("snoop", "%"+str); } 

int Setup(){
    command::Setup();
    nmsh::Setup();
    TermInfo = TERMINAL_D->query_term_info(Terminal);
}

int eventFlushQueuedMessages(){
    print_long_string(this_object(),MessageQueue);
    MessageQueue = "";
    return 1;
}

varargs int eventPauseMessages(int x, int exceptions){
    if(exceptions) MessageExceptions = exceptions;
    else MessageExceptions = 0;
    if(x) PauseMessages = 1;
    else {
        if(PauseMessages){
            eventFlushQueuedMessages();
        }
        PauseMessages = 0;
    }
    return PauseMessages;
}

static varargs int PassengerPrint(string msg, mixed arg2, 
        mixed arg3, object *riders){
    object *targs = ({});
    if(riders && sizeof(riders)){
        int i1, rider_source;
        if(!arg2) arg2 = 0;
        if(!arg3) arg3 = 0;
        if(sizeof(riders)){
            if(arg2 && intp(arg2)){
                object *tmp_riders = riders;
                if(arg2 & MSG_CONV || arg2 & MSG_ENV){
                    foreach(object ob in previous_object(-1)){
                        if(member_array(ob,riders) != -1){
                            tmp_riders -= ({ ob });
                            rider_source = 1;
                        }
                    }
                }
                if((arg2 & MSG_CONV))  true();
                else {
                    if(objectp(arg2)) targs = tmp_riders - ({ arg2 });
                    else if(arrayp(arg2)) targs =  tmp_riders - arg2;
                    else targs = tmp_riders;
                    targs->eventPrint(msg, arg3);
                }
            }
            i1 = sizeof(previous_object(-1)) -1;
            if(i1 < 0) i1 = 0;
            if(sizeof(previous_object(-1)) &&
                    (member_array(previous_object(),riders) != -1 ||
                     member_array(previous_object(-1)[i1],riders) != -1) &&
                    (!intp(arg2) || (!(arg2 & MSG_CONV) && !(arg2 & MSG_ENV))) && 
                    member_array(this_object(),previous_object(-1)) == -1){ 
                if(objectp(arg2)) targs = riders - ({ arg2 });
                else if(arrayp(arg2)) targs = riders - arg2;
                else targs = riders;
                environment()->eventPrint(msg, arg2, arg3);
            }
        }
    }  
    return 1;
}

varargs int eventPrint(string msg, mixed arg2, mixed arg3){
    int msg_class;
    string prompt = "";
    object *passengers = filter(all_inventory(this_object()), (: living :) );
    if( !msg ) return 0;
    if(this_object()->GetProperty("reprompt")){
        prompt = this_object()->GetPrompt(1);
        //I don't want to talk about this.
        //if(this_object()->GetProperty("gmud")) msg = "\n" + msg;
    } 
    if( !arg2 && !arg3 ) msg_class = MSG_ENV;
    else if( !arg2 ){
        if( !intp(arg3) ) msg_class = MSG_ENV;
        else msg_class = arg3;
    }
    else if( !intp(arg2) ) msg_class = MSG_ENV;
    else msg_class = arg2;
    if(sizeof(passengers) && (msg_class & MSG_ENV || msg_class & MSG_CONV)){
    } 
    if( !(msg_class & MSG_NOBLOCK) && GetBlocked("all") ) return 0;

    if((msg_class & MSG_ANNOYING) && annoyblock) return 0;

    /* This is no longer necessary, since the commands
     * "mute" and "gag" can now keep things quiet on
     * on channels for individuals if they so wish.
     * if((msg_class & MSG_CHAN) && environment() &&
     *  environment()->GetProperty("meeting room")) return 0;
     */
    if( GetLogHarass() )
        log_file("harass/" + GetKeyName(), strip_colours(msg) + "\n");
    if( !TermInfo )
        TermInfo = TERMINAL_D->query_term_info(GetTerminal());

    if(this_object()->GetParanoia("cursefilter")){
        msg = FILTER_D->eventFilter(msg, "curse");
    }
    if( !(msg_class & MSG_NOCOLOUR) ){
        int indent;
        //Uncomment below to enable indentation of "conversation"
        //if( msg_class & MSG_CONV ) indent = 4;
        //else indent = 0;
        if( msg_class & MSG_NOWRAP )
            msg = terminal_colour(msg + "%^RESET%^", TermInfo);
        else
            msg = terminal_colour(msg + "%^RESET%^\n" + prompt, TermInfo,
                    GetScreen()[0], indent);
    }
    else if( !(msg_class & MSG_NOWRAP) ) msg = wrap(msg, GetScreen()[0]-1);
    if(PauseMessages && !(msg_class & MessageExceptions)){
        MessageQueue += msg;
    }
    else {
        if(msg_class & MSG_RECEIVE){
            receive(msg);
            return 1;
        }
        if( Client ){
            eventReceive("<"+msg_class+" "+msg+" "+msg_class+">\n");
        }
        else {
            eventReceive(msg);
        }
    }
    return 1;
}

varargs int SetBlocked(string type, int flag){
    if( !type ) return 0;
    if( undefinedp(flag) ) flag = !Blocked[type];
    if( Blocked[type] == 2 && !archp(this_player()) ){
        this_player()->eventPrint("Unable to unblock " + type + ".");
        return -1;
    }
    Blocked[type] = flag;
    message("system", "You are "+(Blocked[type] ? "now blocking" :
                "no longer blocking")+" "+type+".", this_object());
    return Blocked[type];
}

int GetBlocked(string type){ return (Blocked["all"] || Blocked[type]); }

int SetClient(int x){
    return 0;
    if( x ) SetTerminal("unknown");
    return (Client = x);
}

int GetClient(){ return Client; }

int SetLogHarass(int x){
    string txt;

    if( GetForced() || (this_player(1) != this_object()) ) return LogHarass;
    if( LogHarass == x ) return LogHarass;
    if( x ){
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

int GetLogHarass(){ return LogHarass; }

int *SetScreen(int width, int height){ 
    if(!width) width = (__LARGEST_PRINTABLE_STRING__-1)/50;
    if(!height) height = __LARGEST_PRINTABLE_STRING__/width;

    width--;
    if( width * height > __LARGEST_PRINTABLE_STRING__ ){
        if( width > height ) width = __LARGEST_PRINTABLE_STRING__/height;
        else if( height > width ) height = __LARGEST_PRINTABLE_STRING__/width;
        else {
            width = (__LARGEST_PRINTABLE_STRING__-1)/50;
            height = 50;
        }
    }
    return (Screen = ({ width, height })); 
}

int *GetScreen(){ return Screen; }

string SetTerminal(string terminal){ 
    switch( terminal ){
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
        case "html" : terminal = "html"; break;
        default:
        log_file("terminals", "Unknown terminal type: " + terminal + "\n");
        terminal = Terminal;
        break;
    }
    if( terminal != Terminal ) 
        TermInfo = TERMINAL_D->query_term_info(terminal);
    return Terminal = terminal;
}

string GetTerminal(){ return Terminal; }

string GetKeyName(){ return 0; }

int SetAnnoyblock(int i){
    if(!this_player()) return 0;
    if(archp(this_object()) && !archp(this_player())) return 0;
    if(!archp(this_object()) && this_player() != this_object()) return 0;
    if(i) annoyblock = 1;
    else annoyblock = 0;
    return annoyblock;
}

int GetAnnoyblock(){
    return annoyblock;
}

static int rArrow(string str){
    int ret, cedmode = this_object()->GetCedmode();
    switch(cedmode){
        case 0 : ret = nmsh::rArrow(str); break;
        case 1 : ret = cedit::rArrow(str); break;
    }
    return ret;
}

static int rCtrl(string str){
    int ret, cedmode = this_object()->GetCedmode();
    switch(cedmode){
        case 0 : ret = nmsh::rCtrl(str); break;
        case 1 : ret = cedit::rCtrl(str); break;
    }
    return ret;
}

static int rBackspace(){
    int ret, cedmode = this_object()->GetCedmode();
    switch(cedmode){
        case 0 : ret = nmsh::rBackspace(); break;
        case 1 : ret = cedit::rBackspace(); break;
    }
    return ret;
}

static int rEnter(){
    int ret, cedmode = this_object()->GetCedmode();
    switch(cedmode){
        case 0 : ret = nmsh::rEnter(); break;
        case 1 : ret = cedit::rEnter(); break;
    }
    return ret;
}

static int rAscii(string str){
    int ret, cedmode = this_object()->GetCedmode();
    switch(cedmode){
        case 0 : ret = nmsh::rAscii(str); break;
        case 1 : ret = cedit::rAscii(str); break;
    }
    return ret;
}

static int rDel(){
    int ret, cedmode = this_object()->GetCedmode();
    switch(cedmode){
        case 0 : ret = nmsh::rDel(); break;
        case 1 : ret = cedit::rDel(); break;
    }
    return ret;
}

static int rAnsi(string str){
    int ret, cedmode = this_object()->GetCedmode();
    switch(cedmode){
        case 0 : ret = nmsh::rAnsi(str); break;
        case 1 : ret = cedit::rAnsi(str); break;
    }
    return ret;
}
