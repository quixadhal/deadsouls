/*    /verbs/items/read.c
 *    from the Dead Souls Mud Library
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
    SetRules("OBJ", "OBJ at OBJ", "STR in OBJ","STR on OBJ", "STR from OBJ",  
            "STR of OBJ");
    SetErrorMessage("Read something?");
    SetHelp("Syntax: <read ITEM>\n"
            "        <read ITEM on OBJECT>\n"
            "        <read ITEM from OBJECT>\n\n"
            "Allows you to read an object with text on it if you have "
            "the proper intelligence.\n\n"
            "See also: listen, look, search, smell, touch");
}

mixed can_read_obj() {
    return check_light();
}

mixed can_read_obj_at_obj() {
    return check_light();
}

mixed can_read_str_word_obj() {
    return check_light();
}

mixed do_read_obj(object ob) {
    return ob->eventRead(this_player());
}

mixed do_read_obj_at_obj(object ob, object target) {
    return ob->eventRead(this_player(), target);
}

mixed do_read_str_on_obj(string str, object ob) {
    if(ob) return ob->eventRead(this_player(), str);
}

mixed do_read_str_in_obj(string str, object ob) {
    if(ob) return ob->eventRead(this_player(), str);
}

mixed do_read_str_of_obj(string str, object ob) {
    return do_read_str_in_obj(str, ob);
}

//This function courtesy of Manchi
mixed do_read_str_from_obj(string str, object ob) {
    return do_read_str_in_obj(str, ob);
}
