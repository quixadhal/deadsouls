/*   
 *    Help for the pwd command (located inside the player object)
 */

#include <lib.h>

inherit LIB_DAEMON;

void help() {
    message("help", "Syntax: pwd\n\n"
            "Defined internal to the user object.\n"
            "Prints your current working directory."
            "\n\n"
            "See also: cd", this_player());
}
