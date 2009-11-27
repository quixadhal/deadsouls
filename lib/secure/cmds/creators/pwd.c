/*   
 *    Help for the pwd command (located inside the player object)
 */

#include <lib.h>

inherit LIB_DAEMON;

string GetHelp() {
    return ("Syntax: pwd\n\n"
            "Defined internal to the user object.\n"
            "Prints your current working directory.\n"
            "See also: cd");
}
