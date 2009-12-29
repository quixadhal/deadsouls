#include <daemons.h>
#include <lib.h>

inherit LIB_COMMAND;

int cmd(string str){
    int i, cols = 2;
    string *ret;
    if(str) i = atoi(str);
    if(i) cols = i;
    ret = explode(format_page(sefuns(), cols), "\n");
    this_player()->eventPage(ret);
    return 1;
}

string GetHelp(string str) {
    return "Syntax: sefuns [COLUMNS]\n\n"
        "Displays all available simulated efuns. If a number is given, the "
        "output is broken up into the number of columns specified."
        "\nSee also efun, efuns, sefun, lfun, lfuns, showfuns, findfun";
}
