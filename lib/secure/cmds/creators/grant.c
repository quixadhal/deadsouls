/*    /secure/cmd *    from Dead Souls 3.2.2
 *    access granting command to allow cres to give out private access
 *    created by Descartes of Borg 940918
 */

#include <lib.h>

inherit LIB_DAEMON;

int cmd(string str) {
    string *tmp;
    string type, file, who, path;

    if(!str) return notify_fail("Syntax: grant [access] to [who] on [file]\n");
    else if(sscanf(str, "%s to %s on %s", type, who, file) != 3)
      return notify_fail("Syntax: grant [access] to [who] on [file]\n");
    if(sizeof(tmp = explode(file, "/")) < 2)
      return notify_fail("You cannot grant such access.\n");
    if(tmp[0] != "domains" && tmp[0] != "realms")
      return notify_fail("You cannot grant such access.\n");
    path = "/"+tmp[0]+"/"+tmp[1]+"/";
    if(file_size(path+"adm") != -2) mkdir(path+"adm");
    file = absolute_path((string)this_player()->query_cwd(), file);
    if(!file_exists(path+"adm/access.c"))
      write_file(path+"adm/access.c", "#include <lib.h>\n\ninherit LIB_ACCESS;\n");
    if(file_size(file) == -2 && file[strlen(file)-1] != '/') file = file + "/";
    if(type == "read" || type == "all") {
        if(!((int)call_other(path+"adm/access","grant_access","read",file,who)))
          return notify_fail("Read access grant failed!\n");
        if( file_size(file) == -2 ) {
            if( !((int)call_other(path + "adm/access", "grant_access", "read",
                file[0..<2], who)) )
            return notify_fail("Read access grant failed.\n");
        }
    }
    if(type == "write" || type == "all") {
        if(!((int)call_other(path+"adm/access","grant_access","write",file,who)))
          return notify_fail("Write access grant failed!\n");
        if( file_size(file) == -2 ) {
            if( !((int)call_other(path + "adm/access", "grant_access", "write",
                file[0..<2], who)) )
            return notify_fail("Write access grant failed.\n");
        }
    }
    message("system",capitalize(type)+" access granted to "+who+" on "+file+".",
      this_player());
    return 1;
}

void help() {
    message("help", "Syntax: <grant [access] to [whom] on [file|directory]>\n\n"
      "Allows you to give other creators access to files or directories "
      "under your control.  The access parameter is \"read\", "
      "\"write\", or \"all\".", this_player());
}
