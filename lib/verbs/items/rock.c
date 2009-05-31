#include <lib.h>

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("rock");
    SetRules("","OBJ", "WRD OBJ", "STR on OBJ");
    SetErrorMessage("Rock what? Or rock what on what?");
    SetHelp("Syntax: <rock OBJECT>\n"
            "        <rock THING on OBJECT>\n\n"
            "Allows you to rock an object, or perhaps a thing on the "
            "object.  For example, you might want to "
            "\"rock the halo\", or simply \"rock in rocking chair\".\n\n"
            "");
}

mixed can_rock(){
    return 1;
}

mixed can_rock_obj(){
    return this_player()->CanManipulate();
}

mixed can_rock_wrd_obj(){
    return this_player()->CanManipulate();
}

mixed can_rock_str_on_obj(){
    return this_player()->CanManipulate();
}

mixed do_rock(){
    object furn = this_player()->GetProperty("furniture_object");
    if(furn && objectp(furn)){
        return furn->eventRock(this_player());
    }
    write("You rock back and forth.");
    say(this_player()->GetCapName()+" rocks back and forth.");
    return 1;
}

mixed do_rock_obj(object target){
    return target->eventRock(this_player());
}

mixed do_rock_wrd_obj(string wrd, object target){
    return target->eventRock(this_player());
}

varargs mixed do_rock_str_on_obj(string thing, object target){
    return target->eventRock(this_player(), remove_article(lower_case(thing)));
}
