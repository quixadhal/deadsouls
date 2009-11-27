/*    /cmd *    from Dead Souls Mudlib 3.2
 *    a command to access the man documents for efuns and such
 *    created by Descartes of Borg 940122
 */

#include <lib.h>
#include <dirs.h>

inherit LIB_DAEMON;

#define MAN_PAGES ({ "sefun", "efun/all", "applies", "lfun/all" })

int cmd(string str) {
    string d, tmp;
    string *pages;
    int i;

    if(!str) return notify_fail("Usage: man <function>\n");
    i = sizeof(MAN_PAGES);
    pages = ({});
    while(i--) 
        if(file_exists(tmp = sprintf("%s/%s/%s", DIR_DOCS, MAN_PAGES[i],
                        str))) pages += ({ tmp });
    if(!(i = sizeof(pages))) return notify_fail("No such man page.\n");
    else if(i > 1) 
        message("system", "Showing only the first of "+i+" man pages.",
                this_player());
    this_player()->eventPage(pages[0]);
    return 1;
}

string GetHelp() {
    return ("Syntax: man <FUNCTION>\n\n"
            "This command provides information on the function specified, "
            "if it has a help file in the /doc/efun/all, /doc/sefun, or "
            "/doc/lfun/all directories.");
}
