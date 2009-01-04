#include <lib.h>
#include <message_class.h>

inherit LIB_DAEMON;

int cmd() {

    write(
            "%^RED%^RED\t%%^^RED%%^^\n"
            "%^GREEN%^GREEN\t%%^^GREEN%%^^\n"
            "%^ORANGE%^ORANGE\t%%^^ORANGE%%^^\n"
            "%^YELLOW%^YELLOW\t%%^^YELLOW%%^^\n"
            "%^BLUE%^BLUE\t%%^^BLUE%%^^\n"
            "%^CYAN%^CYAN\t%%^^CYAN%%^^\n"
            "%^MAGENTA%^MAGENTA\t%%^^MAGENTA%%^^\n"
            "%^BLACK%^BLACK\t%%^^BLACK%%^^\n"
            "%^WHITE%^WHITE\t%%^^WHITE%%^^\n"
            "%^B_RED%^B_RED\t%%^^B_RED%%^^\n"
            "%^B_GREEN%^B_GREEN\t%%^^B_GREEN%%^^\n"
            "%^B_ORANGE%^B_ORANGE\t%%^^B_ORANGE%%^^\n"
            "%^B_YELLOW%^B_YELLOW\t%%^^B_YELLOW%%^^\n"
            "%^B_BLUE%^B_BLUE\t%%^^B_BLUE%%^^\n"
            "%^B_CYAN%^B_CYAN\t%%^^B_CYAN%%^^\n"
            "%^B_BLACK%^B_BLACK\t%%^^B_BLACK%%^^\n"
            "%^B_WHITE%^B_WHITE\t%%^^B_WHITE%%^^\n"
            "%^B_MAGENTA%^B_MAGENTA%^RESET%^\n\n"
            "Special tags: %%^^BOLD%%^^ and %%^^FLASH%%^^ and %%^^RESET%%^^\n\n"
            "You can mix and match, for example: \n"
            "%%^^B_RED%%^^%%^^CYAN%%^^%%^^BOLD%%^^%%^^FLASH%%^^Foo!%%^^RESET%%^^:"
            "%^B_RED%^%^CYAN%^%^BOLD%^%^FLASH%^Foo!%^RESET%^" 
            );
    return 1;
}

string GetHelp() {
    return("Syntax: colors\n\n"
            "Lists all available colors in the corresponding color."
            "\n\n");
}

