/*    /verbs/items/look.c
 *    from the Dead Souls Mud Library
 *    look STR
 *    look at ITEM
 *    created by Descartes of Borg 951218
 *    Version: @(#) look.c 1.6@(#)
 *    Last modified: 96/11/14
 */

#include <lib.h>
#include <message_class.h>
#include <daemons.h>
#include "include/look.h"

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("look");
    SetRules("", "STR", "on OBJ", "OBJ", "at STR", "at OBJ", "in OBJ", "inside OBJ",
            "at OBJ:v in OBJ", "at OBJ:v inside OBJ", "at OBJ on OBJ", "at STR on OBJ");
    SetErrorMessage("Look at or in something?");
    SetHelp("Syntax: <look>\n"
            "        <look at ITEM>\n"
            "        <look in CONTAINER>\n"
            "        <look at ITEM in CONTAINER>\n"
            "        <look at ITEM on ITEM>\n\n"
            "Without any arguments, this command allows you to see a "
            "description of the area about you, including what other "
            "things are there with you.\n\n"
            "If you look at something, then you get a detailed description "
            "of the thing at which you are looking.  You should be able to "
            "look at any thing you see mentioned in the room when you use the "
            "look command without arguments.  Anything you cannot look at is "
            "considered a bug.\n\n"
            "See also: peer");  
}

mixed can_look() {
    if( !environment(this_player()) ) return "You are nowhere.";
    else return check_light();
}

mixed can_look_str(string str, string verb) {
    return can_look_at_str(str, verb);
}

mixed can_look_obj(string verb, string id) {
    return can_look_at_obj(verb, id);
}

mixed can_look_at_str(string str, string verb) {
    if( !environment(this_player()) ) return "You are nowhere.";
    if( SEASONS_D->GetLong(str) == 0 ) {
        //return "There is no " + remove_article(str) + " here.";
    }
    else {
        return check_light();
    }
}

mixed can_look_at_obj(string verb, string id) {
    return check_light();
}

mixed can_look_on_obj(string verb, string id) {
    return can_look_at_obj(verb, id);
}

mixed can_look_in_obj(string verb, string id) {
    return can_look_inside_obj(verb, id);
}

mixed can_look_inside_obj(string verb, string id) {
    return can_look();
}

mixed can_look_at_obj_word_obj(string verb, string targ, string store) {
    return can_look();
}

mixed can_look_at_obj_on_obj(object targ, object where, string id1, string id2){
    return can_look();
}

mixed can_look_at_str_on_obj(string targ, string verb, string id1, string id2){
    return can_look();
}


mixed do_look() {
    if(environment(this_player()) && !this_player()->GetInvis() &&  
            !environment(this_player())->GetProperty("meeting room"))
        environment(this_player())->eventPrint(this_player()->GetName() +
                " looks around.", this_player(), (MSG_ENV|MSG_ANNOYING));
    this_player()->eventDescribeEnvironment(0);
    return 1;
}

varargs mixed do_look_obj(object ob, mixed *args...) {
    return do_look_at_obj(ob, args...);
}

mixed do_look_str(string str) {
    return do_look_at_str(str);
}

varargs mixed do_look_at_obj(object ob, mixed arg) {
    if(ob->GetInvis() && !archp(this_player()) && 
            base_name(ob) != LIB_BASE_DUMMY && !inherits(LIB_BASE_DUMMY,ob) ){
        write("There is no "+arg+" here.");
        return 1;
    }
    return ob->eventShow(this_player());
}

varargs mixed do_look_on_obj(object ob, mixed arg){
    return do_look_at_obj(ob, arg);
}

mixed do_look_at_str(string str) {
    //if( SEASONS_D->GetLong(str) == 0 ) {
    //	return "There is no " + remove_article(str) + " here.";
    //    }
    return SEASONS_D->eventShow(this_player(),
            remove_article(lower_case(str)));
}

mixed do_look_in_obj(object ob, mixed arg){ 
    return do_look_inside_obj(ob, arg); 
}

mixed do_look_inside_obj(object ob,mixed arg) {
    if(ob->GetInvis() && !archp(this_player()) &&
            base_name(ob) != LIB_BASE_DUMMY && !inherits(LIB_BASE_DUMMY,ob) ){
        return write("There is no "+arg+" here.");
    }
    return ob->eventShowInterior(this_player());
}

mixed do_look_at_obj_word_obj(object target, object storage, mixed arg) {
    if(target->GetInvis() && !archp(this_player()) &&
            base_name(target) != LIB_BASE_DUMMY && !inherits(LIB_BASE_DUMMY,target) ){
        return write("There is no "+arg+" here.");
    }
    return target->eventShow(this_player());
}

varargs mixed do_look_at_str_on_obj(string id, object ob) {
    return ob->eventShow(this_player(), remove_article(lower_case(id)));
}
