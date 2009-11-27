/* /secure/cmd * from the Dead Souls LPC Library
 * gives information on leaked memory
 * NOTE: Only works with DEBUGMALLOC defined
 * created by Descartes of Borg 950206
 */

#include <lib.h>

inherit LIB_DAEMON;

string cmd(string unused) {
    string ret = "";
    if(efun_exists("check_memory")){
#ifdef __DEBUGMALLOC__
#ifdef __DEBUGMALLOC_EXTENSIONS__
#ifdef __PACKAGE_DEVELOP__
        ret = check_memory(1);
#endif
#endif
#endif
        write("%^RED%^The output for this command may be misleading. It is kept "+
                "as a historical curiosity, but is not necessarily reliable.%^RESET%^");
        if(sizeof(ret) < 7000) write(ret);
        else {
            this_player()->eventPage(({ret}));
        }
        return "%^RED%^The output for this command may be misleading. It is kept "+
            "as a historical curiosity, but is not necessarily reliable.%^RESET%^";
    }
    else {
        return "This command relies on an efun that is not currently available.";
    }
}

string GetHelp() {
    return ("Syntax: memcheck\n\n"
            "Gives you information about leaked memory.\n"
            "See also: mstatus, netstat");
}
