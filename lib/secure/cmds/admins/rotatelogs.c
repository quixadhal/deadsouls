#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

int cmd(string arg) {
    int ret;
    int force = 0;
    if(arg) force = 1;
    ret = LOG_D->RotateLogs(force);
    if(ret){
        write("Logs rotated.");
    }
    else {
        write("Failed to rotate logs.");
    }
    return 1;
}

string GetHelp(){
    return ("Syntax: rotatelogs <force>\n\n"
            "Archives old logs if they are too large. With an argument, "
            "the archiving occurs regardless of size.");
}
