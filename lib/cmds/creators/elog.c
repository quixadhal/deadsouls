//      /bin/dev/_elog.c
//      from the Dead Souls Mudlib
//      tails error logs
//      created by Descartes of Borg 930813

#include <lib.h>
#include <dirs.h>

inherit LIB_DAEMON;

int cmd(string str) {
    if(!str) str = DIR_ERROR_LOGS+"/"+previous_object()->GetKeyName();
    else str = DIR_ERROR_LOGS+"/"+str;
    write(str+":\n");
    if(!tail(str)) write("No errors in "+str+".\nTry /log/runtime or /log/catch\n");
    return 1;
}

string GetHelp() {
    return ("Syntax: elog [error log]\n\n"
            "This will tail your personal error log if issued without any\n"
            "arguments.  If an argument is given, it searches for a file\n"
            "by that name in the mud's error log directory.\nFor example:\n"
            "elog\nelog town\nelog secure\nelog lib\n\nSee also: "
            "log, replog");
}
