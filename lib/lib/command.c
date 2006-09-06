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
private static int last_cmd_time = 0;
private static int cmd_count = 1;

int direct_force_liv_str() { return 1; }
int direct_force_liv_to_str() { return 1; }


/*  ***************  /lib/command.c driver applies  ***************  */

static void create() {
    apostrophe_exceptions = ({ "ed","modify","delete","create","add" });
    apostrophe_exceptions += ({"rwho", "finger","mudlist","tell","eval"});
    apostrophe_exceptions += ({"say","speak","yell","whisper","shout"});
    //Fix here courtesy of Jonez
    apostrophe_exceptions += ({"ping", "reply"});
    SearchPath = ({ DIR_PLAYER_CMDS, DIR_SECURE_PLAYER_CMDS, DIR_CLAN_CMDS,
      DIR_COMMON_CMDS, DIR_SECURE_COMMON_CMDS });
}

static string process_input(string cmd) { 
    return cmd;
}

/*  ***************  /lib/command.c command lfuns  ***************  */

static int cmdAll(string args) {
    object old_agent;
    mixed err;
    string verb, file;

    if(MAX_COMMANDS_PER_SECOND){
	if(last_cmd_time == time()) cmd_count++;
	else {
	    last_cmd_time = time();
	    cmd_count = 1;
	}
	if(!creatorp(this_player()) && cmd_count > MAX_COMMANDS_PER_SECOND) {
	    write("You have exceeded the "+MAX_COMMANDS_PER_SECOND+" commands per second limit.");
	    return 1;
	}
    }    

    old_agent = this_agent(this_object());
    verb = query_verb();

    if(this_player()->GetSleeping() > 0) {
	if(verb != "wake") {
	    this_player()->eventPrint("You are asleep.");
	    return 1;
	}
    }

    if(query_custom_command(verb) && query_custom_command(verb) != "" && !creatorp(this_player()) ){
	this_player()->eventPrint("How clever of you. Or lucky. In any case, this command is unavailable to you.");
	return 1;
    }
    if( !(file = (query_custom_command(verb) )) || query_custom_command(verb) == "") {
	if( !(file = (string)CMD_D->GetCommand(verb, GetSearchPath())) ) {
	    string cmd;
	    int dbg;

	    if( args ) cmd = verb + " " + args;
	    else cmd = verb;
	    if( (int)this_object()->GetProperty("parse debug") ) dbg = 1;
	    if( (int)this_object()->GetProperty("debug") ) dbg = 1;
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
    if(stringp(val)) {
	if(!strsrch(val,"/secure/cmds/admins") || !strsrch(val,"/cmds/admins")){
	    if(!(int)master()->valid_apply(({ "SECURE", "ASSIST", "LIB_CONNECT" })) ){
		tell_creators("Security violation in progress: "+identify(previous_object(-1)) + ", "+get_stack());
		error("Illegal attempt to modify path data: "+identify(previous_object(-1)) + ", "+get_stack());

	    }
	}
	val = ({ val });
    }

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


