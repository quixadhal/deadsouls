#include <lib.h>

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("move");
    SetRules("OBJ", "STR on OBJ");
    SetSynonyms("shove");
    SetErrorMessage("Move what? Or move what on what?");
    SetHelp("Syntax: <move OBJECT>\n"
            "        <move THING on OBJECT>\n\n"
            "Allows you to move an object, or perhaps a thing on the "
            "object.  For example, you might want to "
            "\"move the painting on the wall\", or simply \"move carpet\".\n\n"
            "Synonyms: shove");
}

mixed can_move_obj() {
    return 1;
}

mixed can_move_str_on_obj() {
    return 1;
}

mixed do_move_obj(object target) {
    return target->eventManipulate(this_player());
}

varargs mixed do_move_str_on_obj(string thing, object target) {
    return target->eventManipulate(this_player(), remove_article(lower_case(thing)));
}
