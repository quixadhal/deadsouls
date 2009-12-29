#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

int cmd() {
    int ret = LOG_D->RotateLogs();
    if(ret){
        write("Logs rotated.");
    }
    else {
        write("Failed to rotate logs.");
    }
    return 1;
}

string GetHelp(){
    return ("Syntax: rotatelogs\n\n"
            "Archives old logs if they are too large.");
}
