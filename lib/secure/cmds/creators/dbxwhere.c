/* Beek - 10/4/94 */
#include <lib.h>

inherit LIB_DAEMON;

mixed cmd(string str) {
    mapping error;
    object ob;
    int i;

    if (str) {
        ob = find_player(str);
        if (!ob) {
            return "Player not found.";
        }
    } else
        ob = this_player();
    if (error = ob->GetLastError()) {
        return sprintf("%s\n", master()->standard_trace(error, 1));
    } else write("No error.\n");
    return "dbxwhere run complete.";
}

string GetHelp(){
    return "Syntax: dbxwhere [person]\n\n"
        "Debugging thingy.\nSee also: dbxframe";
}
