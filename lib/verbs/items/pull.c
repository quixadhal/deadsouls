#include <lib.h>

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("pull");
    SetRules("OBJ", "STR on OBJ", "STR from OBJ");
    SetSynonyms("yank", "haul", "drag");
    SetErrorMessage("Pull what? Or pull what on what?");
    SetHelp("Syntax: <pull OBJECT>\n"
            "        <pull THING on OBJECT>\n\n"
            "Allows you to pull an object, or perhaps a thing on the "
            "object.  For example, you might want to "
            "\"pull the lever on the wall\", or simply \"pull lever\".\n\n"
            "Synonyms: haul, drag, yank");
}

mixed can_pull_obj() {
    return this_player()->CanManipulate();
}

mixed can_pull_str_on_obj() {
    return this_player()->CanManipulate();
}

mixed can_pull_str_from_obj() {
    return this_player()->CanManipulate();
}

mixed do_pull_obj(object target) {
    return target->eventPull(this_player());
}

varargs mixed do_pull_str_on_obj(string thing, object target) {
    return target->eventPull(this_player(), remove_article(lower_case(thing)));
}

varargs mixed do_pull_str_from_obj(string thing, object target) {
    return do_pull_str_on_obj(thing, target);
}
