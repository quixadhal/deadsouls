/*    /secure/ambassador/_head.c
 *    from Dead Souls 3.3
 *    gives the first 22 lines of a file
 *    created by Descartes of Borg 940928
 */

#include <lib.h>

inherit LIB_DAEMON;

int cmd(string str) {
    string *lines;
    string file;
    int x;

    if(!str) return notify_fail("Syntax: <head [file]>\n");
    file = absolute_path((string)this_player()->query_cwd(),str);
    if(!file_exists(file)) return notify_fail("No such file: "+file+"\n");
    else if(!(str = read_file(file)))
      return notify_fail("Empty file: "+file+"\n");
    if((x = sizeof(lines = explode(str, "\n"))) > 22) x = 23;
    str = implode(lines[0..x-1], "\n");
    message("system", str, this_player());
    return 1;
}

void help() {
    message("help", "Syntax: <head [file]>\n\n"
      "Gives you the first 22 lines in the file named.\n\n"
      "See also: cat, more, tail", this_player());
}
