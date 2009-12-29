/*    /secure/cmds/creator/lsed.c
 *    from the Dead Souls LPC Library
 *    an LPC scripted editor
 *    created by Descartes of Borg 950407
 */

#include <lib.h>

inherit LIB_DAEMON;

static void LsedFile(string file, string *cmds, int num_cmds);

mixed cmd(string args) {
    string *files, *lines, *tmp;
    string total, script, pwd;
    int i, maxi, x;

    if( args == "" || !args ) return "Syntax: <lsed [script] [filelist]>";
    if( (maxi = sizeof(files = explode(args, " "))) == 1 )
        return "You must specify the name of a file to run the script on.";
    pwd = this_player()->query_cwd();
    script = absolute_path( pwd, files[0] );
    files = files[1..];
    maxi--;
    for(i=0, tmp = ({}); i<maxi; i++) {
        files[i] = absolute_path(pwd, files[i]);
        tmp += wild_card(files[i]);
    }
    maxi = sizeof(files = tmp);
    if( !(total = read_file(script)) ) return "Failed to load script: "+script;
    x = sizeof(lines = explode(total, "\n"));
    for(i=0; i<maxi; i++){
        int y;

        y = file_size(files[i]);
        if( y == -2 ) {
            message("system", "lsed: File " + files[i] + " is a directory.",
                    this_player());
            continue;
        }
        else if( y == -1 ) {
            message("system", "lsed: File " + files[i] + " does not exist.",
                    this_player());
            continue;
        }
        LsedFile(files[i], lines, x);
    }
    return 1;
}

static void LsedFile(string file, string *cmds, int num_cmds) {
    string ret;
    int i;

    ed_start(file, 0);
    if( query_ed_mode() == -1 ) {
        message("system", "lsed: Failed to read file " + file, this_player());
        return;
    }
    for(i=0; i<num_cmds; i++) {
        ret = ed_cmd(cmds[i]);
        if( query_ed_mode() == -1 ) {
            message("system", "lsed: Script quit unexpectedly on line " +
                    (i+1) + " for file " + file + " with message: " + ret, 
                    this_player());
            return;
        }
    }
    ret = ed_cmd("x");
    if( query_ed_mode() != -1 ) {
        ed_cmd("Q");
        message("system", "lsed: permission denied for " + file,
                this_player());
        return;
    }
    message("system", ret, this_player());
}

string GetHelp() {
    return ("Syntax: lsed <script> <filelist>\n\n"
            "Allows you to perform editor commands on a file or set of files "
            "from a script.  You simply write a script of editor commands "
            "and save it to a file.  For example, say that you wanted to "
            "change set_item_functions() to SetItems() and "
            "set_search_function() to SetSearch() in every file in your home "
            "directory.  You would write a script that looks like:\n\n"
            "s/set_item_functions/SetItems/g\n"
            "s/set_search_function/SetSearch/g\n\n"
            "save it as convert.lsed, and then at the command line type: "
            "\"lsed sconvert.lsed *\".  Keep in mind that the MudOS "
            "driver will only let an execution thread run so long "
            "before automatically terminating it with a too long evaluation "
            "error, so avoid running lsed on a large number of files at "
            "once.\n"
            "See also: ed");
}

