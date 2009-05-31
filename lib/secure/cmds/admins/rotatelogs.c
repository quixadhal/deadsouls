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

void help() {
    write("Syntax: rotatelogs\n\n");
    write("Archives old logs if they are too large.");
}
