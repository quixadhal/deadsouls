#include <daemons.h>
#include <lib.h>

inherit LIB_COMMAND;

int cmd(string str){
    FILE_D->SearchFiles(str);
    return 1;
}

string GetHelp(){
    return "Syntax: find [-s] [-i] <name>\n"
        "This command searches the list of files known to the file daemon for files "
        "matching the specified search parameter. This list of files is refreshed approximately "
        "once every 25 hours automatically, or every time the mud boots, or every time the "
        "file daemon is reloaded.\n"
        "Options:\n"
        "-s Strict matching, aka not substrings.\n"
        "-i Case insensitive\n"
        "Examples\n"
        "find FooBar: finds FooBar, and FooBar.h, but not foobar or FOObar.h\n"
        "find -i FooBar: finds foobar, FooBar, FooBarBaz, and Foobar.c\n"
        "find -s FooBar: finds FooBar, but NOT FooBar.c or foobarbaz\n" 
        "find -i -s FooBar: finds fooBAR, and FooBar, but not FooBar.c\n"
        "";
}
