#include <lib.h>

inherit LIB_DAEMON;

mixed cmd(string arg) {
    int flag = !!arg;
    this_player()->eventPage(explode(mud_status(flag), "\n"));
    return 1;
}

string GetHelp() {
    return "Syntax: mstatus <arg>\n\n"
        "Gives you statistics on the driver and mudlib. "
        "If the optional argument is given, you also get:"
        "\nadd_message statistics"
        "\nHash table of living objects"
        "\nFunction cache information"
        "\nHeart beat information"
        "\nShared string hash table"
        "\nCall out information"
        "\nObject name hash table status"
        "\nSee man: mud_status"
        "\nSee also: netstat, fdinfo, cache, callouts, dumpallobj, opcprof";
}
