#include <lib.h>

inherit LIB_DAEMON;

int cmd(string arg){
    malloc_status();
    return 1;
}

string GetHelp(){
    return "Syntax: malloc\n\n"
        "Lists memory usage statistics. Output will "
        "depend on memory management package specified in options.h"
        " when the driver is compiled."
        "\nSee man: malloc_status";
}
