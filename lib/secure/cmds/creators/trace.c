#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

mixed cmd(string args) {
    int x;
#ifdef __TRACE__
    if(!args) args = "0";
    x = atoi(args);
    if(!archp(previous_object())) return 0;
    trace(x);
    write("Set tracing level to "+x+".");
#endif
    return 1;
}

string GetHelp() {
    string ret = "";
#ifdef __TRACE__
    ret = "Syntax: trace [level]\n\n"
        "Displays function call tracing to you. See \"man trace\""
        " for details on tracing levels.";
#else
    ret = "This command is disabled.\n"
        "See \"man trace\""
        " for details.";
#endif

    return ret;
}

