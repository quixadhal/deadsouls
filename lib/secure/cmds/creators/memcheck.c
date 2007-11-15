/* /secure/cmd * from the Dead Souls LPC Library
 * gives information on leaked memory
 * NOTE: Only works with DEBUGMALLOC defined
 * created by Descartes of Borg 950206
 */

#include <lib.h>

inherit LIB_DAEMON;

string cmd(string unused) {
    string ret = check_memory(1);
    string tmpfile = generate_tmp();
    write("%^RED%^The output for this command may be misleading. It is kept "+
      "as a historical curiosity, but is not necessarily reliable.%^RESET%^");
    if(sizeof(ret) < 7000) write(ret);
    else {
        write_file(tmpfile,ret);
        this_player()->eventPage(tmpfile);
    }
    rm(tmpfile);
    return "%^RED%^The output for this command may be misleading. It is kept "+
    "as a historical curiosity, but is not necessarily reliable.%^RESET%^";
}

void help() {
    message("help", "Syntax: <memcheck>\n\n"
      "Gives you information about leaked memory. "
      "\n\n" 
      "See also: mstatus, netstat", 
      this_player());
}
