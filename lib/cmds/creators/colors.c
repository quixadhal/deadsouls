#include <lib.h>
#include <message_class.h>

inherit LIB_DAEMON;

int cmd() {

    write(
      "%^RED%^RED\n"
      "%^GREEN%^GREEN\n"
      "%^ORANGE%^ORANGE\n"
      "%^YELLOW%^YELLOW\n"
      "%^BLUE%^BLUE\n"
      "%^CYAN%^CYAN\n"
      "%^MAGENTA%^MAGENTA\n"
      "%^BLACK%^BLACK\n"
      "%^WHITE%^WHITE\n"
      "%^B_RED%^B_RED\n"
      "%^B_GREEN%^B_GREEN\n"
      "%^B_ORANGE%^B_ORANGE\n"
      "%^B_YELLOW%^B_YELLOW\n"
      "%^B_BLUE%^B_BLUE\n"
      "%^B_CYAN%^B_CYAN\n"
      "%^B_BLACK%^B_BLACK\n"
      "%^B_WHITE%^B_WHITE%^RESET%^\n"
      "%^B_MAGENTA%^B_MAGENTA%^RESET%^\n"
    );
    return 1;
}

string GetHelp() {
    return("Syntax: colors\n\n"
      "Lists all available colors in the corresponding color."
      "\n\n");
}

