#include <lib.h>

inherit LIB_DAEMON;

mixed cmd(string str) {
    return @EndText
        |---------------------------------------------------------------|-------------|
EndText;
}

void help() {
    write("Syntax: margins\n");
    write(@EndText
This simple command prints a line to help creators when writing
descriptions.  It gives a visual indicator of when to stop on
one line and continue on the next.
EndText
    );
}
