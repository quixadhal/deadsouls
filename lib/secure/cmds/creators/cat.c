/*    /secure/cmds/creator/cat.c
 *    from the Foundation II LPC Library
 *    command to display file contents
 *    created by Descartes of Borg 940124
 *    some updates made 950915
 */

#include <lib.h>

inherit LIB_DAEMON;

mixed cmd(string str) {
    string *arr;
    string tmp;

    if( !str ) return "You must specify a file to cat.";
    else str = absolute_path((string)this_player()->query_cwd(), str);
    if( !file_exists(str) ) return "File " + str + " not found.";
    else if( !(tmp = read_file(str)) )
      return "Unable to read file " + str + ".";
    if( sizeof(arr = explode(tmp, "\n")) > 100 ) 
      tmp = implode(arr[0..99], "\n") + "\n\t***  TRUNCATED  ***";
    message("system", tmp, this_player());
    return 1;
}

int help() {
    message("help", "Syntax: <cat [file]>\n\n"
      "Displays the contents of the file mentioned all ate once.",
      this_player());
}
