/*    /cmd *    from Dead Souls
 *    command to handle channel privs
 *    created by Descartes of Borg 940428
 */

#include <lib.h>

inherit LIB_DAEMON;

int cmd(string str) {
    string opt, who, channel;
    object ob;

    if(!str) return 0;
    if(!archp(previous_object())) return 0;
 
    // Attempt to remove or add a players line rights.
    if(sscanf(str, "%s %s %s", opt, who, channel) == 3) {
      if(!(ob = find_player(lower_case(who)))) 
        return notify_fail("No such player online.\n");
      if(opt == "remove") {
          if(member_array(channel, ob->GetChannels()) == -1) {
            this_player()->eventPrint(ob->GetName()
              + " is not currently subscribed to the " + channel + " line.");
            return 1;
          }
          ob->RestrictChannel(channel);
          this_player()->eventPrint("%^RED%^You restrict " + ob->GetName()
            + " from the " + channel + " line.%^RESET%^");
          ob->eventPrint("%^RED%^You have lost your " + channel
            + " line rights.%^RESET%^");
          return 1;
      }
      else if(opt == "add") {
          if(member_array(channel, ob->GetRestrictedChannels()) == -1) {
            message("system", ob->GetName() + " is not currently restricted from the "
              + channel + " line.", this_player());
            return 1;
          }
          ob->UnrestrictChannel(channel);
          this_player()->eventPrint("%^RED%^You unrestrict " + ob->GetName()
            + " from the " + channel + " line.%^RESET%^");
          ob->eventPrint("%^RED%^You have regained your " + channel
            + " line rights.%^RESET%^");
          return 1;
      }
    }
    // Otherwise, return restriction information, if it is requested.
    else if(str) {
      string *channels;
      string tmp;
      int size;

      if(!(ob = find_player(lower_case(str)))) 
        return notify_fail("No such player online.\n");
      channels = ob->GetRestrictedChannels();
      size = sizeof(channels);
      tmp = ob->GetName() + " is currently restricted from ";
      if(size) tmp += "the " + conjunction(channels);
      else tmp += "no";
      if(size == 1) tmp += " line.";
      else tmp += " lines.";
      this_player()->eventPrint(tmp);
      return 1;
    }
    else return 0;
}

void help() {
    message("help",
      "Syntax: channel add|remove <player> <channel>\n"
      "        channel <player>\n"
      "\n"
      "Used to restrict a player from using a line, or to allow them back "
      "onto that line. You can also request a listing of a players currently "
      "restricted lines with this command.", this_player());
}
