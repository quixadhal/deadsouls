/*  /cmds/secure/admin/notify
 *  from the Dead Souls V Mud Library
 *  created by Blitz@Dead Souls 950115
 */
 
#include <lib.h>
#include <daemons.h>
 
 
varargs private void eventNotify(object who, string text);
 
mixed cmd(string str) {
    object ob = previous_object();
    string who;
    if( !ob ) return 0;
    who = (string)ob->GetKeyName();
    if( sizeof(str) ) {
        eventNotify(ob, str);
        return 1;
    }
    ob->eventPrint("[ Dead Souls Notification System ]");
    ob->eventPrint("Please compose your notification now. \n");
    ob->eventEdit(DIR_TMP + "/" +who+ ".notice", (: eventNotify, ob, 0 :));
    return 1;
}
 
varargs private void eventNotify(object who, string text) {
    int x;
    string file, str;
    if( !who ) return;
    if( !text ) {
        string * lines;
        file = DIR_TMP + "/" +(string)who->GetKeyName() + ".notice";
        if( file_size(file) < 1 ) return;
        str = read_file(file);
        str = implode(filter(lines = explode(str, "\n"), (: sizeof :)), "\n");
        rm(file);
    } else str = text;
    if( !sizeof(str) ) return;
    x = (int)NOTIFY_D->eventAddNotice(who, str);
    if( x )
      who->eventPrint("Your notice [id #"+(x-1)+"] has been posted!");
    else
      who->eventPrint("Error adding notice.");
}
 
string GetHelp(string str) {
    return ("Syntax: notify\n"
            "        notify <message>\n\n"
            "This command allows users with proper access to "
            "add a message to a database of notifications.  "
            "All new notifications are displayed to any creator "
            "upon login.  Typing \"notify\" alone will put you "
            "into the text editor.  Otherwise, the passed "
            "message will be used.\n\n");
}
