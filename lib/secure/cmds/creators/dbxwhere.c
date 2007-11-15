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
            //write("No such player.");
            return "Player not found.";
        }
    } else
        ob = this_player();
    if (error = (mapping)ob->GetLastError()) {
        return sprintf("%s\n", master()->standard_trace(error, 1));
    } else write("No error.\n");
    return "dbxwhere run complete.";
}
