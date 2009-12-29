/*    /verbs/rooms/jump.c
 *    from the Dead Souls Mud Library
 *    jump in OBJ
 *    jump into OBJ
 *    jump off OBJ
 *    jump off of OBJ
 *    jump from OBJ
 *    jump through OBJ
 *    jump across OBJ
 *    jump over OBJ
 *    jump onto OBJ
 *    jump on OBJ
 *    go ahead and jump
 *    might as well jump
 *    created by Descartes of Borg 960117
 */

#include <lib.h>
#include <jump.h>
#include "include/jump.h"

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("jump");
    SetRules("in OBJ", "into OBJ", "off OBJ", "off of OBJ", "from OBJ",
            "through OBJ", "across OBJ", "over OBJ", "onto OBJ", "on OBJ"); 
    SetSynonyms("leap");
    SetErrorMessage("Jump from, into, over, on, or through something?");
    SetHelp("Syntax: jump into <PLACE>\n"
            "        jump off <PLACE>\n"
            "        jump through <THING>\n"
            "        jump over <THING>\n"
            "        jump on <THING>\n\n"
            "These six different syntaxes actually represent three very "
            "different types of motion.  They are thus often used in very "
            "different circumstances.  All of them do, however, allow you "
            "to jump.\n"
            "Synonyms: leap\n"
            "See also: enter, go");
}

mixed can_jump_word_obj(){
    if(!stringp(hobbled(this_player()))){
        return "Your injuries prevent that movement.";
    }
    return 1;
}

mixed can_jump_word_word_obj(){
    if(!stringp(hobbled(this_player()))){
        return "Your injuries prevent that movement.";
    }
    return 1;
}

varargs mixed do_jump_in_obj(object ob, mixed *args...){
    return do_jump_into_obj(ob, args...);
}

varargs mixed do_jump_into_obj(object ob, mixed *args...){
    string id;
    id = remove_article(lower_case(args[0]));
    return ob->eventJump(this_player(), id, JUMP_INTO);
}

varargs mixed do_jump_off_obj(object ob, mixed *args...){
    return do_jump_from_obj(ob, args...);
}

varargs mixed do_jump_off_of(object ob, mixed *args...){
    return do_jump_off_of_obj(ob, args...);
}

varargs mixed do_jump_from_obj(object ob, mixed *args...){
    string id;
    id = remove_article(lower_case(args[0]));
    return ob->eventJump(this_player(), id, JUMP_FROM);
}

varargs mixed do_jump_through_obj(object ob, mixed *args...){
    string id;
    id = remove_article(lower_case(args[0]));
    return ob->eventJump(this_player(), id, JUMP_THROUGH);
}

varargs mixed do_jump_across_obj(object ob, mixed *args...){
    return do_jump_over_obj(ob, args...);
}

varargs mixed do_jump_over_obj(object ob, mixed *args...){
    string id;
    id = remove_article(lower_case(args[0]));
    return ob->eventJump(this_player(), id, JUMP_OVER);
}

varargs mixed do_jump_onto_obj(object ob, mixed *args...){
    return do_jump_on_obj(ob, args...);
}

varargs mixed do_jump_on_obj(object ob, mixed *args...){
    string id;
    id = remove_article(lower_case(args[0]));
    return ob->eventJump(this_player(), id, JUMP_ON);
}    
