/*    /secure/cmds/admins/addadverb.c
 *    From the Dead Souls Mud Library
 *    Command for adding new adverbs to the soul
 *    Created by Descartes of Borg 961214
 *    Version: @(#) addadverb.c 1.1@(#)
 *    Last modified: 96/12/14
 */

#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

mixed cmd(string args) {
    string array adverbs;

    if( !args || args == "" ) {
        return "Add which adverb?";
    }
    adverbs = map(explode(args, ","), (: trim :));
    if( !SOUL_D->AddAdverbs(adverbs...) ) {
        previous_object()->eventPrint("Addition of adverbs failed.");
        return 1;
    }
    previous_object()->eventPrint("Adverbs added.");
    return 1;
}

string GetHelp(string args) {
    return ("Syntax: addadverb ADVERB1,..,ADVERBn\n\n"
            "Allows you to add 1 or more adverbs separated by commas to "
            "the system adverbs that can be used as adverbs in most soul "
            "commands.\n"
            "See also: addemote,removeadverb,removeemote");
}
