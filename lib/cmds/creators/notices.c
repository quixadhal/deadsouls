/*  /cmds/creators/notices
 *  from the Dead Souls V Mud Library
 *  created by Blitz@Dead Souls 950115
 */
 
#include <lib.h>
#include <daemons.h>
 
#define OneDay  (3600 * 24)
 
string GetHelp(string foo);
 
mixed cmd(string str) {
    string * words;
    object ob = this_player();
    int x;
    if( !sizeof(str) ) x = 1;
    else x = to_int(str);
    if( x < 1 ) {
        if( !archp(this_player()) ) return GetHelp(0);
        words = explode(str, " ");
        if( words[0] == "-d" ) {
            if( sizeof(words) < 2 ) return GetHelp(0);
            x = to_int(words[1]);
            if( x < 1 && words[1] != "0" )
              return "Bad value ("+words[1]+").";
            if( NOTIFY_D->eventRemoveNotice(x) )
              ob->eventPrint("Notice number " + x + " has been erased.");
              else ob->eventPrint("Could not remove number " + x + ".");
            return 1;
        }
        else if( words[0] == "-p" && sizeof(words) > 1 ) {
            string file = words[1];
            if( sizeof(words) > 2 ) x = to_int(words[2]);
            else x = 1;
            if( x < 1 ) return GetHelp(0);
            if( NOTIFY_D->eventWriteNotices(file, time() - (OneDay*x)) )
              ob->eventPrint("Notices dumped to " + file + ".");
              else ob->eventPrint("No notices found, nothing done.");
            return 1;
        }
        else return GetHelp(0);
    }
    x = time() - (OneDay * x);
    if( !(int)NOTIFY_D->eventPrintNotices(this_player(), x) )
      return "No recent notices found.";
      else return 1;
}
 
string GetHelp(string foo) {
    string str;
    str =   "Syntax: notices\n"
            "        notices <number of days back>\n\n"
            "Without arguments, this command displays all notices "
            "posted within the last 24 hours.  You may also "
            "specify how many days back to search.  For example, "
            "\"notices 7\" will display all notices posted within "
            "the last week.\n\n";
    if( archp(this_player()) )
      str += "Arch Commands: notices -d <id number>\n"
             "               notices -p <file name> <number of days back>\n\n"
             "The d option deletes the specified notice id number.\n"
             "the p option will dump the output into <file name>\n";
    return str;
}
