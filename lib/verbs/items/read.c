/*    /verbs/items/read.c
 *    from the Dead Souls V Object Library
 *    created by Descartes of Borg 960121
 *    Version: @(#) read.c 1.2@(#)
 *    Last modified: 96/10/15
 */

#include <lib.h>
#include <daemons.h>
#include <function.h>
 
inherit LIB_VERB;
 
static void create() {
    verb::create();
    SetVerb("read");
    SetRules("OBJ", "STR on OBJ", "STR from OBJ", "STR of OBJ");
    SetErrorMessage("Read something?");
    SetHelp("Syntax: <read ITEM>\n"
	    "        <read ITEM on OBJECT>\n"
	    "        <read ITEM from OBJECT>\n\n"
	    "Allows you to read an object with text on it if you have "
	    "the proper intelligence.\n\n"
	    "See also: listen, look, search, smell, touch");
}
 
mixed can_read_obj() {
    return 1;
}


mixed can_read_str_word_obj() {
    return 1;
}

mixed do_read_obj(object ob) {
    return ob->eventRead(this_player());
}

mixed do_read_str_word_obj(string str, object ob) {
    return ob->eventRead(this_player(), str);
}
