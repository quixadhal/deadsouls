/*    /verbs/items/look.c
 *    from the Dead Souls Object Library
 *    look STR
 *    look at ITEM
 *    created by Descartes of Borg 951218
 *    Version: @(#) look.c 1.6@(#)
 *    Last modified: 96/11/14
 */

#include <lib.h>
#include <daemons.h>
#include "include/look.h"

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("look");
    SetRules("", "STR", "OBJ", "at STR", "at OBJ", "in OBJ", "inside OBJ",
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
    //tc("1");
    if( !environment(this_player()) ) return "You are nowhere.";
    else return check_light();
}

mixed can_look_str(string str, string verb) {
    //tc("2");
    return can_look_at_str(str, verb);
}

mixed can_look_obj(string verb, string id) {
    //tc("3");
    return can_look_at_obj(verb, id);
}

mixed can_look_at_str(string str, string verb) {
    //tc("thingy");
    if( !environment(this_player()) ) return "You are nowhere.";
    if( SEASONS_D->GetLong(str) == 0 ) {
	//return "There is no " + remove_article(str) + " here.";
    }
    else {
	return check_light();
    }
}

mixed can_look_at_obj(string verb, string id) {
    //tc("5");
    return check_light();
}

mixed can_look_in_obj(string verb, string id) {
    //tc("6");
    return can_look_inside_obj(verb, id);
}

mixed can_look_inside_obj(string verb, string id) {
    //tc("7");
    return can_look();
}

mixed can_look_at_obj_word_obj(string verb, string targ, string store) {
    //tc("8");
    return can_look();
}

mixed can_look_at_obj_on_obj(object targ, object where, string id1, string id2){
    //tc("9");
    return can_look();
}

mixed can_look_at_str_on_obj(string targ, string verb, string id1, string id2){
    //tc("10");
    return can_look();
}


mixed do_look() {
    //tc("do look","red");
    if(environment(this_player()) && !this_player()->GetInvis() &&  
      !environment(this_player())->GetProperty("meeting room"))
	environment(this_player())->eventPrint((string)this_player()->GetName() +
	  " looks around.", this_player());
    this_player()->eventDescribeEnvironment(0);
    return 1;
}

varargs mixed do_look_obj(object ob, mixed *args...) {
    //tc("do look obj","green");
    return do_look_at_obj(ob, args...);
}

mixed do_look_str(string str) {
    //tc("do look str","blue");
    return do_look_at_str(str);
}

varargs mixed do_look_at_obj(object ob, mixed arg) {
    //tc("do look at obj","yellow");
    if(ob->GetInvis() && !archp(this_player()) && 
      base_name(ob) != LIB_DUMMY && !inherits(LIB_DUMMY,ob) ){
	write("There is no "+arg+" here.");
	return 1;
    }
    return ob->eventShow(this_player());
}

mixed do_look_at_str(string str) {
    //tc("do look at str","white");
    //if( SEASONS_D->GetLong(str) == 0 ) {
    //	return "There is no " + remove_article(str) + " here.";
    //    }
    return (mixed)SEASONS_D->eventShow(this_player(),
      remove_article(lower_case(str)));
}

mixed do_look_in_obj(object ob, mixed arg){ 
    //tc("do look in obj","cyan");
    return do_look_inside_obj(ob, arg); 
}

mixed do_look_inside_obj(object ob,mixed arg) {
    //tc("do look inside obj","red");
    if(ob->GetInvis() && !archp(this_player()) &&
      base_name(ob) != LIB_DUMMY && !inherits(LIB_DUMMY,ob) ){
	return write("There is no "+arg+" here.");
    }
    return (mixed)ob->eventShowInterior(this_player());
}

mixed do_look_at_obj_word_obj(object target, object storage, mixed arg) {
    //tc("arg: "+identify(arg));
    if(target->GetInvis() && !archp(this_player()) &&
      base_name(target) != LIB_DUMMY && !inherits(LIB_DUMMY,target) ){
	return write("There is no "+arg+" here.");
    }
    return (mixed)target->eventShow(this_player());
}

varargs mixed do_look_at_str_on_obj(string id, object ob) {
    //tc("sadface");
    return (mixed)ob->eventShow(this_player(), remove_article(lower_case(id)));
}
