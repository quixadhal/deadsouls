/*    /secure/cmds/admins/link.c
 *    from the Dead Souls V Object Library
 *    link SECONDARY to PRIMARY
 *    link PLAYER
 *    link
 *    created by Descartes of Borg 951216
 */

#include <lib.h>
#include <link.h>
#include <daemons.h>
#include <message_class.h>

inherit LIB_DAEMON;

mixed cmd(string args) {
  mixed tmp;
  string primary, secondary;

  if( !archp(previous_object()) ) return 0;

  // If no arguments are specified, dump all links. 
  if( !args || args == "" ) {
    mapping links;
    string formatString;
    int screenSize = ((int *)this_player()->GetScreen())[0];
    int playerSize = ((screenSize * 4) / 16) -1;
    int emailSize = ((screenSize * 6) / 16) - 1;
    int lastOnSize = ((screenSize * 6) / 16) - 1;

    if(playerSize < 14) playerSize = 14;
    if(emailSize < 25) emailSize = 25;
    if(lastOnSize < 25) lastOnSize = 25;

    formatString = "%:-" + playerSize + "s %:-" + emailSize
      + "s %:-" + lastOnSize + "s";

    tmp = ({ center("%^CYAN%^  " + mud_name()
      + " Approved Character Links%^YELLOW%^", screenSize) });
    
    tmp += ({ sprintf(formatString, "Player", "Email", "Last On%^RESET%^") });

    links = (mapping)CHARACTER_D->GetLinks();
    foreach(string p in sort_array(keys(links), 1)) {   
      class char_link l;
	  int maxi;

      l = links[p];
      if( !(maxi = sizeof(l->Secondaries)) ) continue;
      tmp += ({ sprintf(formatString, capitalize(p), l->Email,
        ((l->LastOnWith == p) ? ctime(l->LastOnDate) : "unknown")) });
      foreach(string pl in l->Secondaries)
        tmp += ({ sprintf(formatString, "  " + capitalize(pl), "",
          ((l->LastOnWith == pl) ? ctime(l->LastOnDate) : "unknown")) });
	}
	this_player(1)->eventPage(tmp, MSG_SYSTEM);
	return 1;
  }
  
  // Or, link a secondary to a primary.
  else if( sscanf(args, "%s to %s", secondary, primary) == 2 ) {
    this_player(1)->eventPrint("Email for player: ", MSG_PROMPT);
    input_to(function(string email, string primary, string secondary) {
      mixed tmp;

      if( !email || email == "" ) {
        this_player(1)->eventPrint("Aborted.", MSG_SYSTEM);
        return;
      }
      tmp = (mixed)CHARACTER_D->eventLink(primary, secondary, email);
      if( !tmp ) this_player(1)->eventPrint("Failed.", MSG_SYSTEM);
      else if( tmp == 1) this_player(1)->eventPrint("Linked.", MSG_SYSTEM);
      else this_player(1)->eventPrint(tmp, MSG_SYSTEM);
    }, primary, secondary);
    return 1;
  }
 
  // Otherwise, get link information on a specific player.
  else {
    mapping mp;
    string str;
 
    mp = (mapping)CHARACTER_D->GetLink(convert_name(args));
    if( !mp ) this_player()->eventPrint(capitalize(args) + " has no "
      "links listed.", MSG_SYSTEM);
    else {
      str = "Primary: " + capitalize(mp["primary"]) + "\n";
      str += "Last on " + ctime(mp["last on"]) + " with " +
        capitalize(mp["last char"]) + "\n";
      str += "Secondaries: " + implode(mp["secondaries"], ",");
      this_player()->eventPrint(str, MSG_SYSTEM);
    }
    return 1;
  }
}

string GetHelp(string str) {
    return ("Syntax: <link SECONDARY to PRIMARY>\n"
	    "        <link PLAYER>\n"
	    "        <link>\n\n"
	    "In the first form, it allows you to mark two characters "
	    "as being controlled by the same real individual.  It "
	    "creates a relationship where one character is considered "
	    "primary, and the other is secondary.  If link relationships "
	    "already exist for one or more, this will alter those "
	    "and create a new one with the primary one you specify as "
	    "primary.  For example, if both characters were already set "
	    "up as primary links, the one you specify with this command "
	    "as primary becomes the sole primary link for these characters, "
	    "and the secondary plus its secondaries all become secondaries "
	    "fot his primary.\n\n"
	    "If you just pass a player name as an argument, this command "
	    "will show you the link information on that player.\n\n"
	    "If you give no arguments, then this command will list all "
	    "known links/\n\n"
	    "See also: finger, unlink");
}
