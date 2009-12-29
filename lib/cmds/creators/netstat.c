/*    /cmds/creator/netstat.c
 *    from the Dead Souls LPC Library
 *    gives socket status information
 *    created by Descartes of Borg 921123
 */

#include <lib.h>

inherit LIB_DAEMON;

mixed cmd(string args) {
    string *arr;
    string ret = "";
    string dss = replace_string(dump_socket_status(),"-- 0","--\n 0");
    foreach(mixed key, mixed val in network_stats()){
        val = val+"";
        ret += sprintf("%:-30s %s",key, val)+"\n";
    }
    write(ret);

    arr = filter(explode(dss, "\n"), (: $1[0..1] != "-1" :));
    this_player()->eventPage(arr, "system");
    return 1;
}

string GetHelp() {
    return ("Syntax: netstat\n\n"
            "Gives you information about sockets being used by the LPC "
            "server through the MudOS socket efuns.\n"
            "See also: callouts, dumpallobj, mstatus");
}
