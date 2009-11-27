/*    /cmd *    from Dead Souls
 *    help for the cd command (located inside the player object)
 *    created by Descartes of Borg 940723
 */

#include <lib.h>

inherit LIB_DAEMON;

string GetHelp() {
    return ("Syntax: cd [directory]\n\n"
            "Defined internal to the user object.\n"
            "Changes your current working directory to the directory "
            "you specify.  If no directory is specified, it will make your "
            "home directory current.\n"
            "See also: pwd, mkdir, rmdir");
}
