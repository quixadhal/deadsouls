/*  /verbs/items/bait.c
 *  from the Dead Souls V Object Library
 *  bait verb used with /lib/bait.c
 *  created by Blitz@Dead Souls 960117
 */
 
#pragma save_binary

#include <lib.h>
#include "include/bait.h"
 
inherit LIB_VERB;
 
static void create() {
    verb::create();
    SetVerb("bait");
    SetRules("OBJ with OBJ");
    SetErrorMessage("Bait what with what?");
    SetHelp("Syntax: bait <POLE> with <BAIT>\n\n"
            "The bait command is normally used to fasten bait onto "
            "a fishing pole.  The POLE must be a device of some "
            "kind which is used for catching fish.  The BAIT "
            "must be some sort of fishing bait.");
}

mixed can_bait_obj_with_obj(string verb) { return 1; }
 
mixed do_bait_obj_with_obj(object pole, object bait) {
    if( pole == bait ) {
        this_player()->eventPrint("You can't use it to bait itself!");
	return 1;
    }
    return (mixed)bait->eventBait(this_player(), pole);
}
