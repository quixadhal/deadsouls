#include <lib.h>

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("knock");
    SetRules("OBJ", "on OBJ", "STR on OBJ");
    SetSynonyms("rap");
    SetErrorMessage("Knock on what? Or knock what on what?");
    SetHelp("Syntax: <knock on OBJECT>\n"
            "        <knock THING on OBJECT>\n\n"
            "Allows you to knock on an object, or perhaps a thing on the "
            "object.  For example, you might want to "
            "\"knock on the window on the wall\", or simply \"knock on door\".\n\n"
            "Synonyms: rap");
}

mixed can_knock_obj() {
    return this_player()->CanManipulate();
}

mixed can_knock_on_obj() {
    return can_knock_obj();
}

mixed can_knock_str_on_obj() {
    return this_player()->CanManipulate();
}

mixed can_knock_on_str_on_obj() {
    return can_knock_str_on_obj();
}

mixed do_knock_obj(object target) {
    return target->eventKnock(this_player());
}

mixed do_knock_on_obj(object target) {
    return do_knock_obj(target);
}

varargs mixed do_knock_str_on_obj(string thing, object target) {
    return target->eventKnock(this_player(), remove_article(lower_case(thing)));
}

varargs mixed do_knock_on_str_on_obj(string thing, object target) {
    return do_knock_str_on_obj(thing, target);
}
