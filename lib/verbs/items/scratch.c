#include <lib.h>

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("scratch");
    SetRules("OBJ", "on OBJ", "STR on OBJ");
    SetErrorMessage("Scratch on what? Or scratch what on what?");
    SetHelp("Syntax: <scratch on OBJECT>\n"
            "        <scratch THING on OBJECT>\n\n"
            "Allows you to scratch on an object, or perhaps a thing on the "
            "object.  For example, you might want to "
            "\"scratch on the window on the wall\", or simply \"scratch on door\".\n\n"
            "Synonyms: rap");
}

mixed can_scratch_obj() {
    return 1;
}

mixed can_scratch_on_obj() {
    return can_scratch_obj();
}

mixed can_scratch_str_on_obj() {
    return 1;
}

mixed can_scratch_on_str_on_obj() {
    return can_scratch_str_on_obj();
}

mixed do_scratch_obj(object target) {
    return target->eventScratch(this_player());
}

mixed do_scratch_on_obj(object target) {
    return do_scratch_obj(target);
}

varargs mixed do_scratch_str_on_obj(string thing, object target) {
    return target->eventScratch(this_player(), remove_article(lower_case(thing)));
}

varargs mixed do_scratch_on_str_on_obj(string thing, object target) {
    return do_scratch_str_on_obj(thing, target);
}
