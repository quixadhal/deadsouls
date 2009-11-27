/*    /cmds/creator/showtree.c
 *    idea borrowed from LIMA for Dead Souls
 *    shows information about an object
 *    created by Beek@Dead Souls
 */

#include <lib.h>
#include "include/showtree.h"

inherit LIB_DAEMON;

private string Error;

mixed cmd(string str) {
    string func, file;
    object tmp;

    if( !str || str == "" )
        return "Syntax: <showtree FILE> or <showtree FUNCTION in FILE>";
    if( sscanf(str, "%s in %s", func, file) != 2 ) {
        if( sscanf(str, "%s %s", func, file) != 2 ) {
            func = 0;
            file = str;
        }
    }	
    tmp = get_object(file);
    file = absolute_path(this_player()->query_cwd(), file);
    if(!file_exists(file) && !file_exists(file+".c")){
        if(tmp)
            file = base_name(tmp);
    }
    if(!file_exists(file) && !file_exists(file+".c")){
        write("Cannot find the file specified.");
        return 1;
    }
    str = ShowTree(file, func, 0);
    if( !str ) return Error;
    this_player()->eventPage(explode(str, "\n"), "system");
    return 1;
}

string ShowTree(string file, string func, int indent) {
    object ob;
    string str;
    int found;

    if( strlen(file) > 2 && file[<2..] == ".c" ) file = file[0..<3];
    if( file[0] != '/' ) file = "/" + file;
    if( !(ob = load_object(file)) ) {
        Error = "Error in loading " + file + ".";
        return 0;
    }
    str = sprintf("%*-' 's" + file, indent*4, "");
    if( func && (function_exists(func, ob) == file) )
        str += " (" + func + " defined)\n";
    else str += "\n";
    if( !func || function_exists(func, ob) ) found = 1;
    indent++;
    foreach(file in inherit_list(ob)) {
        string tmp;

        tmp = ShowTree(file, func, indent);
        if( tmp && tmp != "" ) {
            str += tmp;
            found = 1;
        }
        else if( !tmp ) return 0;
    }
    return (found ? str : "");
}

string GetHelp() {
    return ("Syntax: showtree <FILE | FUNC in FILE>\n\n"
            "In its first version, shows you the full inheritance "
            "tree for the named object.  In its second form, it shows "
            "you all files in the inheritance tree which contain the "
            "function you name, specifically noting those objects which "
            "have definitions for the function.\n"
            "See also: help");
}
