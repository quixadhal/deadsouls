/*    /secure/cmds/players/history.c
 *    from the Dead Souls Object Library
 *    command to view history information
 *    created by Descartes of Borg 960511
 */

#include <lib.h>
#include <message_class.h>

inherit LIB_DAEMON;

mixed cmd(string args) {
    int x;
    object player = this_player();
    if(!player) return 0;
    if(player->GetForced()) return 0;

    if( x = to_int(args) ) {
        if( previous_object()->SetHistorySize(x) != x ) {
            if( x < MIN_HISTORY_SIZE )
                return "History size must be at least " + MIN_HISTORY_SIZE + ".";
            else if( x > MAX_HISTORY_SIZE )
                return "History size can be no larger than " + MAX_HISTORY_SIZE +
                    ".";
            else return "Invalid history size.";
        }
        else {
            previous_object()->eventPrint("History size set to: " + x);
        }
    }
    else {
        string array arr, tmp = ({});
        mapping history;
        int cmd_num, i;

        history = this_player()->GetCommandHist();
        cmd_num = sizeof(history);
        foreach(mixed key, mixed cmd in history) {
            if( !cmd ) continue;
            tmp += ({ sprintf("%:-5d %s", key, cmd) });
        }
        previous_object()->eventPage(tmp, MSG_SYSTEM);
    }
    return 1;
}

string GetHelp(string str) {
    return ("Syntax: history\n"
            "        history <SIZE>\n\n"
            "This command lets you check your command history.  A command " 
            "history is kept for the commands you enter, allowing you "
            "quick access to execute or modify previous commands.  The size "
            "of your command buffer defaults to " + MIN_HISTORY_SIZE + " but "
            "may be expanded to " + MAX_HISTORY_SIZE + " if you desire.  You "
            "can change the size of your history buffer by passing the size "
            "as an argument to the history command.  Without arguments, the "
            "command simply prints the commands in your history buffer.\n\n"
            "The following syntaxes are available for accessing previously "
            "issued commands:\n"
            "\t!!\t\texecute the last command\n"
            "\t!#\t\texecute command number #\n"
            "\t!-#\t\texecute #th command prior\n"
            "\t^old^new\treplace 'old' with 'new' in last command and "
            "execute\n"
            "\t!#^old^new\tsame as above, except for command number #\n"
            "\t!#s/old/new/g\tsame as above, except replacing all "
            "occurrences of 'old'\n\n"
            "See also: alias, nickname");
}
