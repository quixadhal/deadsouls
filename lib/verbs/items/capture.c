/*    /verbs/items/capture.c
 *    From the Dead Souls V Object Library
 *    A verb to allow people to capture or catch things with an object
 *    Creatd by Descartes of Borg 961010
 *    Version: @(#) capture.c 1.4@(#)
 *    Last modified: 96/10/10
 */

#include <lib.h>

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("capture");
    SetRules("LIV with OBJ", "LIV in OBJ");
    SetSynonyms("catch", "trap");
    SetErrorMessage("Capture what with what?");
    SetHelp("Syntax: <capture OBJECT with OBJECT>\n\n"
	    "Some objects can be used to trap living things.  This command "
	    "therefore allows you to capture those things.\n\n"
	    "Synonyms: catch, trap\n\n"
	    "See also: free");
}

mixed can_capture_liv_word_obj() {
    return 1;
}

mixed do_capture_liv_word_obj(object target, string word, object tool) {
    return tool->eventCapture(this_player(), target);
}
