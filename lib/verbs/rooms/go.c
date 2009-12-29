/*    /verbs/rooms/go.c
 *    from the Dead Souls Mud Library
 *    go STR
 *    go into STR
 *    created by Descartes of Borg 951104
 */

#include <lib.h>
#include "include/go.h"

inherit LIB_VERB;

static void create(){
    verb::create();
    SetVerb("go");
    SetRules("STR", "into STR");
    SetErrorMessage("Go in which direction?");
    SetHelp("Syntax: go <DIRECTION>\n"
            "        go into <PLACE>\n\n"
            "Moves you towards the direction you specify, or into the place "
            "you specify.  The command \"go into\" is synonymous with the "
            "\"enter\" command.  Note that most directions are pre-aliased "
            "for you to the proper \"go\" commands.  For example, "
            "\"go south\" has been pre-aliased to \"s\".\n"
            "See also: climb, enter, jump, fly, swim");
}

mixed can_go_str(string str){
    if( !environment(this_player()) ) return "You are nowhere.";
    if( this_player()->GetStaminaPoints() <3 )
        return "You are too tired to go anywhere right now.";
    if(!stringp(hobbled(this_player()))) return "Your injuries prevent easy movement. Perhaps you should try crawling.";
    return environment(this_player())->CanGo(this_player(), str);
}

mixed can_go_into_str(string str){
    if( !environment(this_player()) ) return "You are nowhere.";
    if( this_player()->GetStaminaPoints() <3 )
        return "You are too tired right now.";
    if(!stringp(hobbled(this_player()))) return "Your injuries prevent easy movement. Perhaps you should try crawling.";
    return environment(this_player())->CanEnter(this_player(), str);
}

mixed do_go_str(string str){
    return environment(this_player())->eventGo(this_player(), str);
}

mixed do_go_into_str(string str){
    return environment(this_player())->eventEnter(this_player(), str);
}
