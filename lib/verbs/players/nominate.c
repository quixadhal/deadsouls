/*    /verbs/players/nominate.c
 *    from the Dead Souls  Object Library
 *    Allows players to vote
 *    created by Kalinash 961021
 *    Version: @(#) nominate.c 1.1@(#) Last modified: 96/10/21
 */

#pragma save_binary

#include <lib.h>
#include "include/nominate.h"

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("nominate");
    SetRules("STR");
    SetErrorMessage("Who would you like to nominate?");
    SetHelp("Syntax: <nominate STR>\n"
            "Nominates a candidate");
}

mixed can_nominate_str() {
    object env;
    mixed  err;

    if( !(env = environment(this_player())) ) return 0;
    err = (mixed)env->CanNominate( this_player() );
    if( !err ) return "This doesn't seem the proper place for it.";
    else return err;
}

mixed do_nominate_str( string str ) {
    object env;

    if( !str || !(env = environment(this_player())) ) return 0;
        return (mixed)env->eventNominate( this_player(), str );
}
