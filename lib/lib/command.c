/*    /lib/command.c
 *    from the Dead Soulsr1 Object Library
 *    handles commands of living objects
 *    created by Descartes of Borg 950323
 *    Version: @(#) command.c 1.2@(#)
 *    Last modified: 96/12/07
 */


#include <lib.h>
#include <daemons.h>
#include "include/command.h"

private static int Forced;
private static string CommandFail;
private static string *SearchPath;

/*  ***************  /lib/command.c driver applies  ***************  */

static void create() {
    SearchPath = ({ DIR_PLAYER_CMDS, DIR_SECURE_PLAYER_CMDS, DIR_GUILD_CMDS,
		DIR_COMMON_CMDS, DIR_SECURE_COMMON_CMDS });
}

static string process_input(string cmd) { return cmd; }

/*  ***************  /lib/command.c command lfuns  ***************  */

static int cmdAll(string args) {
    object old_agent;
    mixed err;
    string verb, file;

    old_agent = this_agent(this_object());
    verb = query_verb();
    if( !(file = (string)CMD_D->GetCommand(verb, GetSearchPath())) ) {
	string cmd;
	int dbg;
	
	if( args ) cmd = verb + " " + args;
	else cmd = verb;
	if( (int)this_object()->GetProperty("parse debug") ) dbg = 1;
	else dbg = 0;
	if( (err = parse_sentence(cmd, dbg)) == 1 ) {
	    this_agent(old_agent || 1);
	    return 1;
	}
	if( err ) {
	    if( err == -1 ) {
		if( !(err = (string)VERBS_D->GetErrorMessage(verb)) &&
		    !(err = (string)SOUL_D->GetErrorMessage(verb)) ) {
		    err = "Such a command exists, but no default "
			"syntax is known.";
		}
	    }
	    if( intp(err) )  /* MudOS bug */ err = "What?";
	    SetCommandFail(err);
	}
	message("error", GetCommandFail(), this_object());
	this_agent(old_agent || 1);
	return 1;
    }
    if( (err = (mixed)call_other(file, "cmd", args)) != 1 ) {
	string cmd;
	
	if( err ) SetCommandFail(err);
	if( !args || args == "" ) cmd = verb;
	else cmd = verb + " " + args;
	if( (err = parse_sentence(cmd)) == 1 ) {
	    this_agent(old_agent || 1);
	    return 1;
	}
	if( !err ) err = GetCommandFail();
	message("error", err, this_object());
	this_agent(old_agent || 1);
	return 1;
    }
    this_agent(old_agent || 1);
    return 1;
}

int cmdDebugAll(string args) {
    object old_agent;
    mixed err;
    string verb, file;

    old_agent = this_agent(this_object());
    verb = query_verb();
    if( !(file = (string)CMD_D->GetCommand(verb, GetSearchPath())) ) {
	string cmd;
	
	if( args ) cmd = verb + " " + args;
	else cmd = verb;
	if( (err = parse_sentence(cmd, 3)) == 1 ) {
	    this_agent(old_agent || 1);
	    return 1;
	}
	if( err ) SetCommandFail(err);
	message("error", GetCommandFail(), this_object());
	this_agent(old_agent || 1);
	return 1;
    }
    if( (err = (mixed)call_other(file, "cmd", args)) != 1 ) {
	string cmd;
	
	if( err ) SetCommandFail(err);
	if( !args || args == "" ) cmd = verb;
	else cmd = verb + " " + args;
	if( (err = parse_sentence(cmd, 3)) == 1 ) {
	    this_agent(old_agent || 1);
	    return 1;
	}
	if( !err ) err = GetCommandFail();
	message("error", err, this_object());
	this_agent(old_agent || 1);
	return 1;
    }
    this_agent(old_agent || 1);
    return 1;
}

/*  ***************  /lib/command.c lfuns  ***************  */

int Setup() {
    enable_commands();
    add_action( (: cmdAll :), "", 1);
}

int eventForce(string cmd) {
    string err;
    int res;

    cmd = process_input(cmd);
    Forced = 1;
    err = catch(res = command(cmd));
    Forced = 0;
    if(err) error(err);
    return res;
}

/*  **********  /lib/command.c data manipulation functions  ********** */

string *AddSearchPath(mixed val) {
    if(stringp(val)) val = ({ val });
    else if(!pointerp(val)) error("Bad argument 1 to AddSearchPath()\n");
    return (SearchPath = distinct_array(SearchPath + val));
}

string *RemoveSearchPath(mixed val) {
    if(stringp(val)) val = ({ val });
    else if(!pointerp(val)) error("Bad argument 1 to RemoveSearchPath()\n");
    return (SearchPath -= val);
}

string *GetSearchPath() { return SearchPath; }

int GetForced() { return Forced; }

string GetClient() { return 0; }

string SetCommandFail(string str) { 
    if( !str || str == "" )
      return (CommandFail = "Try \"help index\" for a list of command types.");
    else return (CommandFail = str);
}

string GetCommandFail() { return CommandFail; }


