#include <lib.h>

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("shake");
    SetRules("OBJ", "STR on OBJ");
    SetSynonyms("jiggle");
    SetErrorMessage("Shake what? Or shake what on what?");
    SetHelp("Syntax: <shake OBJECT>\n"
            "        <shake THING on OBJECT>\n\n"
            "Allows you to shake an object, or perhaps a thing on the "
            "object.  For example, you might want to "
            "\"shake my magic 8 ball\", or simply \"shake ball\".\n\n"
            "Synonyms: jiggle");
}

mixed can_shake_obj() {
    return this_player()->CanManipulate();
}

mixed can_shake_str_on_obj() {
    return this_player()->CanManipulate();
}

mixed do_shake_obj(object target) {
    return target->eventShake(this_player());
}

varargs mixed do_shake_str_on_obj(string thing, object target) {
    return target->eventShake(this_player(), remove_article(lower_case(thing)));
}
