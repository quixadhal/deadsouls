#include <lib.h>

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("swivel");
    SetRules("","OBJ", "WRD OBJ", "STR on OBJ");
    SetSynonyms("spin");
    SetErrorMessage("Swivel what? Or swivel what on what?");
    SetHelp("Syntax: <swivel OBJECT>\n"
            "        <swivel THING on OBJECT>\n\n"
            "Allows you to swivel an object, or perhaps a thing on the "
            "object.  For example, you might want to "
            "\"swivel the gimbal\", or simply \"swivel in chair\".\n\n"
            "Synonyms: spin");
}

mixed can_swivel(){
    return 1;
}

mixed can_swivel_obj(){
    return this_player()->CanManipulate();
}

mixed can_swivel_wrd_obj(){
    return this_player()->CanManipulate();
}

mixed can_swivel_str_on_obj(){
    return this_player()->CanManipulate();
}

mixed do_swivel(){
    object furn = this_player()->GetProperty("furniture_object");
    if(furn && objectp(furn)){
        return furn->eventSwivel(this_player());
    }
    write("You swivel to and fro.");
    say(this_player()->GetCapName()+" swivels to and fro.");
    return 1;
}

mixed do_swivel_obj(object target){
    return target->eventSwivel(this_player());
}

mixed do_swivel_wrd_obj(string wrd, object target){
    return target->eventSwivel(this_player());
}

varargs mixed do_swivel_str_on_obj(string thing, object target){
    return target->eventSwivel(this_player(), remove_article(lower_case(thing)));
}
