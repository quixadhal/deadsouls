/*    /secure/cmds/players/passwd.c
 *    from the Dead Souls  Object Library
 *    passwd
 *    created by Descartes of Borg 951202
 */

#include <lib.h>
#include <flags.h>
#include <message_class.h>
#include "include/passwd.h"

inherit LIB_DAEMON;

mixed cmd(string args) {
    object ob;

    if( (int)previous_object()->GetForced() )
      return "You cannot be forced to change your password.";
    if( args && args != "" ) {
	if( !archp(previous_object()) )
	  return "You may not change other people's passwords.";
	if( !user_exists(args = convert_name(args)) )
	  return "No such user exists.";
	ob = find_player(args);
    }
    else ob = previous_object();
    previous_object()->eventPrint("Changing password for " +
				  (ob ? (string)ob->GetCapName() :
				   capitalize(args)) + " on " +
				  mud_name() + ".", MSG_SYSTEM);
    if( previous_object() == ob ) {
	ob->eventPrint("Old password: ", MSG_PROMPT);
	input_to( (: OldPass :), I_NOECHO | I_NOESC, ob);
    }
    else  {
	previous_object()->eventPrint("New password: ", MSG_PROMPT);
	input_to( (: NewPass :), I_NOECHO | I_NOESC, ob || args);
    }
    return 1;
}

static void OldPass(string pass, object who) {
    string oldpass;
    
    if( who != this_player() ) return;
    if( !pass || pass == "" ) {
	who->eventPrint("\nPassword change failed.", MSG_SYSTEM);
	return;
    }
    oldpass = (string)this_player()->GetPassword();
    if( oldpass != crypt(pass, oldpass) ) {
	who->eventPrint("\nPassword change failed.", MSG_SYSTEM);
	return;
    }
    who->eventPrint("\nNew password: ", MSG_PROMPT);
    input_to((: NewPass :), I_NOECHO | I_NOESC, who);
}

static void NewPass(string pass, mixed who) {
    if( !pass || strlen(pass) < 5 ) {
	this_player()->eventPrint("Password must be at least 5 "
				      "characters, password change failed.",
				      MSG_SYSTEM);
	return;
    }
    this_player()->eventPrint("\nConfirm: ", MSG_PROMPT);
    input_to( (: ConfirmPass :), I_NOECHO | I_NOESC, who, pass);
}

static void ConfirmPass(string str, mixed who, string newpass) {
    if( str != newpass ) {
	this_player()->eventPrint("Passwords do not match.", MSG_SYSTEM);
	return;
    }
    if( objectp(who) ) who->SetPassword(crypt(newpass, 0));
    else {
	string *lines;

	who = save_file(who) + __SAVE_EXTENSION__;
	lines = explode(unguarded((: read_file, who :)),"\n");
	unguarded((: rm, who :));
	foreach(string line in lines) {
	    string val;
	    
	    if( sscanf(line, "Password %s", val) )
	      line = "Password \"" + crypt(newpass, 0) + "\"";
	    unguarded((: write_file, who, line + "\n" :));
	}
    }	    
    this_player()->eventPrint("\nPassword changed.", MSG_SYSTEM);
}

string GetHelp(string str) {
    return ("Syntax: <passwd>\n"
	    "        <passwd PLAYER>\n\n"
	    "If you are not an arch, then the second syntax is not available "
	    "to you.  This command allows you to change your password.\n\n"
	    "See also: chfn");
}
	    
