/*    /secure/cmds/creator/tail.c
 *    from the Foundation II Object Library
 *    Displays the last screenful of information from a file
 *    created by Descartes of Borg 950915
 */

#include <lib.h>

inherit LIB_DAEMON;

mixed cmd(string args) {
    string *lines;
    string buff;
    int scr;

    if( !args ) return "You must specify a file to tail.";
    else args = absolute_path((string)this_player()->query_cwd(), args);
    if( !file_exists(args) ) return "File " + args + " not found.";
    if( !(buff = read_file(args)) ) return "Unable to tail " + args + ".";
    scr = (((int *)this_player()->GetScreen())[1] || 24);
    if( scr > 100 ) scr = 100;
    if( sizeof(lines = explode(buff, "\n")) > scr ) 
      buff = implode(lines[<scr..], "\n");
    message("system", buff, this_player());
    return 1;
}

void help() {
    message("help", "Syntax: <tail [file]>\n\n"
	    "Displays the last screenful of information for the specified "
	    "file.\n\n"
	    "See also: cat, head, more.", this_player());
}
