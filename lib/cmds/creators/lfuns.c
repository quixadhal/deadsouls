#include <daemons.h>
#include <lib.h>

inherit LIB_COMMAND;

int cmd(string str){
    int i, cols = 2;
    string *ret, *tmp;
    if(str) i = atoi(str);
    if(i) cols = i;
    tmp = sort_array(FUNCTION_D->GetFunctions(), 1);
    ret = explode(format_page(tmp, cols), "\n");
    this_player()->eventPage(ret);
    return 1;
}

string GetHelp() {
    return "Syntax: lfuns [COLUMNS]\n"
        "Displays lfuns known to the FUNCTION_D. If a number is given, the "
        "output is broken up into the number of columns specified."
        "\nSee also efun, efuns, sefun, lfun, showfuns, findfun";
}
