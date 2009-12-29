#include <lib.h>

inherit LIB_DAEMON;

mixed cmd(string str) {
    if(!creatorp(this_player())) write("Nothing happens.");
    else tell_room(environment(this_player()),"A hollow voice says: \"%^CYAN%^Xyzzy.%^RESET%^\"");
    return 1;
}
