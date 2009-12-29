#include <daemons.h>
#include <lib.h>

inherit LIB_COMMAND;

int cmd(string str){
    int i, cols = 3;
    string *ret;
    if(str) i = atoi(str);
    if(i) cols = i;
    ret = explode(format_page(efuns(), cols), "\n");
    this_player()->eventPage(ret);
    return 1;
}

string GetHelp() {
    return "Syntax: efuns [COLUMNS]\n"
        "Displays all available driver efuns. If a number is given, the "
        "output is broken up into the number of columns specified."
        "\nSee also efun, sefun, sefuns, lfun, lfuns, showfuns, findfun";
}
