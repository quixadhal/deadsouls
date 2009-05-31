#include <lib.h>

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("consult");
    SetRules("OBJ");
    SetErrorMessage("Consult what? Or consult whom?");
    SetHelp("Syntax: <consult OBJECT>\n"
            "        <consult PERSON>\n\n"
            "Allows you to consult an object or a person.\n"
            "\"consult my magic 8 ball\", or simply \"consult ball\".\n\n"
            "");
}

mixed can_consult_obj() {
    return this_player()->CanManipulate();
}

//mixed can_consult_liv() {
//    return this_player()->CanManipulate();
//}

mixed can_consult_str_on_obj() {
    return this_player()->CanManipulate();
}

mixed do_consult_obj(object target) {
    return target->eventConsult(this_player());
}

//mixed do_consult_liv(object target) {
//    return target->eventConsult(this_player());
//}

varargs mixed do_consult_str_on_obj(string thing, object target) {
    return target->eventConsult(this_player(), remove_article(lower_case(thing)));
}
