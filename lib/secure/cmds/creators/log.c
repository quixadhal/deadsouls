//      /bin/dev/_log.c
//      from the Dead Souls Mudlib
//      tails a given log
//      created by Brian@TMI 920128

#include <lib.h>
#include <dirs.h>

inherit LIB_DAEMON;

int cmd(string str) {
    if(!str) {
        notify_fail("Correct syntax: <log [log file]>\n");
        return 0;
    }
    write(str+":\n");
    if(!tail(DIR_LOGS+"/"+str)) write("No such log file: "+str+"\n");
    return 1;
}

string GetHelp(){
    return ("Syntax: log [log file]\n\n"
            "Tails the log file named (i.e. a file "
        "in the mud log directory).\n"
            "See also:\nelog, replog\n");
}
