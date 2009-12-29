#include <lib.h>

inherit LIB_DAEMON;

string GetHelp() {
    return ("Syntax: margins\n\n"
            "This simple command prints a line to help creators when writing "
            "descriptions.  It gives a visual indicator of when to stop on "
            "one line and continue on the next.");
}

mixed cmd(string str) {
    return (
            "|---------------------------------------------------------------|-------------|"
           );

}
