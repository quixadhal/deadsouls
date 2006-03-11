/*    /lib/command.c
 *    from the Dead Souls Object Library
 *    handles commands of living objects
 *    created by Descartes of Borg 950323
 *    Version: @(#) command.c 1.2@(#)
 *    Last modified: 96/12/07
 */


#include <lib.h>
#include <daemons.h>
#include "include/command.h"

//inherit "/lib/props/extra_parse";

private static int Forced;
private static string CommandFail;
private static string *SearchPath;
private static string *apostrophe_exceptions;

/*  ***************  /lib/command.c driver applies  ***************  */

static void create() {
    apostrophe_exceptions = ({ "ed","modify","delete","create","add" });
    apostrophe_exceptions += ({"rwho", "finger","mudlist","tell","eval"});
    apostrophe_exceptions += ({"say","speak","yell","whisper","shout"});
    apostrophe_exceptions += ({"ping"});
    SearchPath = ({ DIR_PLAYER_CMDS, DIR_SECURE_PLAYER_CMDS, DIR_GUILD_CMDS,
      DIR_COMMON_CMDS, DIR_SECURE_COMMON_CMDS });
}

static string process_input(string cmd) { 
    //tc(this_object()->GetName()+": "+identify(parse_command(cmd)));
    return cmd;
}

/*  ***************  /lib/command.c command lfuns  ***************  */

static int cmdAll(string args) {
    object old_agent;
    mixed err;
    string verb, file;

    //if(ParseMe(args) == 1) return 1;
    //tc(this_object()->GetName()+" parse_command_id_list(): "+identify(parse_command_id_list()));
    //tc(this_object()->GetName()+" parse_command_plural_id_list(): "+identify(parse_command_plural_id_list()));
    //tc(this_object()->GetName()+" parse_command_adjectiv_id_list(): "+identify(parse_command_adjectiv_id_list()));
    //tc(this_object()->GetName()+" parse_command_prepos_list(): "+identify(parse_command_prepos_list()));

    if(sizeof(args) && member_array(query_verb(), apostrophe_exceptions) == -1)
	args = replace_string(args,"'","");

    old_agent = this_agent(this_object());
    verb = query_verb();
    //write("verb: "+verb);

    if(this_player()->GetSleeping() > 0) {
	if(verb != "wake") {
	    this_player()->eventPrint("You are asleep.");
	    return 1;
	}
    }

    if(GetCustomCommand(verb) && GetCustomCommand(verb) != "" && !creatorp(this_player()) ){
	this_player()->eventPrint("How clever of you. Or lucky. In any case, this command is unavailable to you.");
	return 1;
    }
    if( !(file = (GetCustomCommand(verb) )) || GetCustomCommand(verb) == "") {
	if( !(file = (string)CMD_D->GetCommand(verb, GetSearchPath())) ) {
	    string cmd;
	    int dbg;

	    if( args ) cmd = verb + " " + args;
	    else cmd = verb;
	    if( (int)this_object()->GetProperty("parse debug") ) dbg = 1;
	    if( (int)this_object()->GetProperty("debug") ) dbg = 1;
	    else dbg = 0;
	    //tc("command: checkpoint 1","green");
	    if( (err = parse_sentence(cmd, dbg)) == 1 ) {
		this_agent(old_agent || 1);
		return 1;
	    }
	    //tc("command: checkpoint 2","green");
	    if( err ) {
		//tc("command: checkpoint 3. err: "+err,"green");
		if( err == -1 ) {
		    //tc("command: checkpoint 4","green");
		    if( !(err = (string)VERBS_D->GetErrorMessage(verb)) &&
		      !(err = (string)SOUL_D->GetErrorMessage(verb)) ) {
			err = "Such a command exists, but no default "
			"syntax is known.";
		    }
		}
		//tc("command: checkpoint 5","green");
		if( intp(err) )  /* MudOS bug */ err = "What?";
		SetCommandFail(err);
	    }
	    //tc("command: checkpoint 6","green");
	    message("error", GetCommandFail(), this_object());
	    this_agent(old_agent || 1);
	    return 1;
	}
	//tc("command: checkpoint 7","green");
    }
    //tc("command: checkpoint 8","green");

    if( (err = (mixed)call_other(file, "cmd", args)) != 1 ) {
	string cmd;

	//tc("command: checkpoint 9","green");
	if( err ) SetCommandFail(err);
	if( !args || args == "" ) cmd = verb;
	else cmd = verb + " " + args;
	if( (err = parse_sentence(cmd)) == 1 ) {
	    //tc("command: checkpoint 10","green");
	    this_agent(old_agent || 1);
	    return 1;
	}
	//tc("command: checkpoint 11","green");
	if( !err ) err = GetCommandFail();
	message("error", err, this_object());
	this_agent(old_agent || 1);
	return 1;
    }
    //tc("command: checkpoint 12","green");
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
    if( !str || str == "" ){
	if(!creatorp(this_player())) CommandFail = "Try \"help commands\" for a list of some commands.";
	if(creatorp(this_player())) CommandFail = "Try \"help creator commands\" for a list of some creator commands.";
	return CommandFail;
    }
    else return (CommandFail = str);
}

string GetCommandFail() { return CommandFail; }


