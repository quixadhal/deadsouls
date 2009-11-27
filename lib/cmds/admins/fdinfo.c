#include <lib.h>

inherit LIB_DAEMON;

int cmd(string arg){
    if(!archp(previous_object())) return 0;
    write(dump_file_descriptors());
    return 1;
}

string GetHelp(){
    return "Syntax: fdinfo\n\n"
        "Lists the status of the machines's file descriptors."
        "\nSee man: dump_file_descriptors"
        "\nSee also: netstat, mstatus, cache, callouts, dumpallobj, opcprof";
}
