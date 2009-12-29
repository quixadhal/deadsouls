#include <lib.h>
#include <message_class.h>

inherit LIB_DAEMON;

int cmd() {
    write(
            "%^RED%^RED\t%%^^RED%%^^\t\t%^BOLD%^%%^^BOLD%%^^%%^^RED%%^^%^RESET%^\n"
            "%^GREEN%^GREEN\t%%^^GREEN%%^^\t%^BOLD%^%%^^BOLD%%^^%%^^GREEN%%^^%^RESET%^\n"
            "%^ORANGE%^ORANGE\t%%^^ORANGE%%^^\t%^BOLD%^%%^^BOLD%%^^%%^^ORANGE%%^^%^RESET%^\n"
            "%^YELLOW%^YELLOW\t%%^^YELLOW%%^^\t%^BOLD%^%%^^BOLD%%^^%%^^YELLOW%%^^%^RESET%^\n"
            "%^BLUE%^BLUE\t%%^^BLUE%%^^\t%^BOLD%^%%^^BOLD%%^^%%^^BLUE%%^^%^RESET%^\n"
            "%^CYAN%^CYAN\t%%^^CYAN%%^^\t%^BOLD%^%%^^BOLD%%^^%%^^CYAN%%^^%^RESET%^\n"
            "%^MAGENTA%^MAGENTA\t%%^^MAGENTA%%^^\t%^BOLD%^%%^^BOLD%%^^%%^^MAGENTA%%^^%^RESET%^\n"
            "%^BLACK%^BLACK\t%%^^BLACK%%^^\t%^BOLD%^%%^^BOLD%%^^%%^^BLACK%%^^%^RESET%^\n"
            "%^WHITE%^WHITE\t%%^^WHITE%%^^\t%^BOLD%^%%^^BOLD%%^^%%^^WHITE%%^^%^RESET%^\n"
            "%^BLACK%^B_RED%^B_RED\t\t\t%%^^B_RED%%^^%^RESET%^\n"
            "%^BLACK%^%^B_GREEN%^B_GREEN\t\t\t%%^^B_GREEN%%^^%^RESET%^\n"
            "%^BLACK%^%^B_ORANGE%^B_ORANGE\t\t%%^^B_ORANGE%%^^%^RESET%^\n"
            "%^BLACK%^%^B_YELLOW%^B_YELLOW\t\t%%^^B_YELLOW%%^^%^RESET%^\n"
            "%^BLACK%^%^B_BLUE%^B_BLUE\t\t\t%%^^B_BLUE%%^^%^RESET%^\n"
            "%^BLACK%^%^B_CYAN%^B_CYAN\t\t\t%%^^B_CYAN%%^^%^RESET%^\n"
            "%^BLACK%^%^B_MAGENTA%^B_MAGENTA\t\t%%^^B_MAGENTA%%^^%^RESET%^\n"
            "%^BOLD%^%^BLACK%^%^B_BLACK%^B_BLACK\t\t\t%%^^B_BLACK%%^^%^RESET%^\n"
            "%^BLACK%^%^B_WHITE%^B_WHITE\t\t\t%%^^B_WHITE%%^^%^RESET%^\n"
            "Special tags: %%^^BOLD%%^^ and %%^^FLASH%%^^ and %%^^RESET%%^^\n\n"
            "You can mix and match, for example: \n"
            "%%^^B_RED%%^^%%^^CYAN%%^^%%^^BOLD%%^^%%^^FLASH%%^^Foo!%%^^RESET%%^^:"
            "%^B_RED%^%^CYAN%^%^BOLD%^%^FLASH%^Foo!%^RESET%^" 
            );
    return 1;
}

string GetHelp() {
    return ("Syntax: colors\n\n"
            "Lists all available colors in the corresponding color.");
}
