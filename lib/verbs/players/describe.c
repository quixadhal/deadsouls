/*  Allows players to describe themselves.
 *  created by Rush@Dead Souls
 */

#include <lib.h>
#include "include/describe.h"

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("describe");
    SetRules("LIV","STR");
    SetErrorMessage("Describe syntax: describe <message>");
    SetHelp("Syntax: describe <MESSAGE>\n\n"
            "Provides a player with a description. You must include "
            "an instance of $N at least once in your description to "
            "denote your name. Example: If I typed 'describe $N is "
            "nondescript.', my description would be 'Rush is "
            "nondescript.'");
}

mixed can_describe_str(string str) {
    if(strsrch(str, "$N") == -1)
        return("Your description must include a minimum of one '$N' "
                + "to specify your name.");
    return 1;
}

mixed do_describe_str(string str) {
    this_player()->SetLong(str);
    write("Ok.");
    return 1;
}
