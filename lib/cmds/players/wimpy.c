/*    /cmds/players/wimpy.c
 *    from the Dead Souls V Object Library
 *    wimpy NUM
 *    wimpy STR
 *    created by Descartes of Borg 960113
 */

#include <lib.h>
#include <message_class.h>

inherit LIB_DAEMON;

mixed cmd(string args) {
    int percentage;
    string cmd;

    if( !args || args == "" ) {
	percentage = (int)this_player()->GetWimpy();
	cmd = (string)this_player()->GetWimpyCommand();
	if( !percentage )
	  this_player()->eventPrint("You have wimpy turned off.", MSG_SYSTEM);
	else this_player()->eventPrint("Percentage: " + percentage + "%\n"
				       "Command: " + cmd, MSG_SYSTEM);
	return 1;
    }
    if( args == "0" ) {
	percentage = 0;
	cmd = 0;
    }
    else if( percentage = to_int(args) ) cmd = 0;
    else if( args == "on" ) {
	percentage = 23;
	cmd = 0;
    }
    else if( args == "off" ) {
	percentage = 0;
	cmd = 0;
    }
    else {
	percentage = 0;
	cmd = args;
    }
    if( !cmd ) {
	if( !percentage ) {
	    this_player()->SetWimpy(0);
	    this_player()->eventPrint("Wimpy is now off!  You are so brave!",
				      MSG_SYSTEM);
	    return 1;
	}
	if( percentage > 30 ) {
	    this_player()->eventPrint("You may not set wimpy greater than "
				      "30%.", MSG_SYSTEM);
	    return 1;
	}
	else if( percentage < 1 ) return "That is not a valid percentage!";
	this_player()->SetWimpy(percentage);
	if( percentage > 20 ) {
	    this_player()->eventPrint("What a weenie!  Get some backbone!",
				      MSG_SYSTEM);
	    return 1;
	}
	else if( percentage > 10 ) {
	    this_player()->eventPrint("You are the cautious one!", MSG_SYSTEM);
	    return 1;
	}
	else {
	    this_player()->eventPrint("You are brave, but not foolish!",
				      MSG_SYSTEM);
	    return 1;
	}
    }
    this_player()->SetWimpyCommand(cmd);
    this_player()->eventPrint("You will execute the command: \"" + cmd +
			      "\" next time you wimpy.", MSG_SYSTEM);
    return 1;
}

string GetHelp(string str) {
    return ("Syntax: <wimpy>\n"
	    "        <wimpy on>\n"
	    "        <wimpy off>\n"
	    "        <wimpy PERCENTAGE>\n"
	    "        <wimpy COMMAND>\n\n"
	    "Wimpy is a system that allows you to automatically execute "
	    "a command when your health points get below a certain percentage "
	    "of your maximum health points.  The wimpy command allows you "
	    "to set which command will be used at which percentage.  "
	    "The \"on\" and \"off\" arguments are simply quick ways to "
	    "set the percentage to 23% and 0% respectively.  You cannot "
	    "Set yourself to wimpy at anything greater than 30%.\n"
	    "If you set go and enter commands, for example:\n"
	    "\tgo west\n"
	    "the wimpy system will first try to use that command, and if it "
	    "fails, it will search for an exit in the room to take.  If "
	    "you set another command as your wimpy command, however, other "
	    "than a go or enter, it will not make any attempt to execute "
	    "some other command.\n"
	    "For example, if you issued the command:\n"
	    "\twimpy gate to Mystery Person\n"
	    "so that \"gate to Mystery Person\" was your wimpy command, "
	    "the wimpy system would try to execute that command when you "
	    "wimpy and will not try any other command, even if the gate "
	    "fails (too low on mp, Mystery Person is not online, etc.).\n"
	    "Without any arguments, the wimpy command displays your current "
	    "settings.\n\n"
	    "See also: attack, status");
}

