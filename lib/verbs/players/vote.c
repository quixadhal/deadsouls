/*    /verbs/players/vote.c
 *    from the Dead Souls  Object Library
 *    Allows players to vote
 *    created by Kalinash 961021
 *    Version: @(#) vote.c 1.1@(#)  Last modified: 96/10/21
 */

#pragma save_binary

#include <lib.h>
#include "include/vote.h"

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("vote");
    SetRules("for STR");
    SetErrorMessage("Who would you like to vote for?");
    SetHelp("Syntax: <vote for STR>\n"
            "Casts your vote for a candidate");
}

mixed can_vote_for_str() {
    object env;
    mixed  err;

    if( !(env = environment(this_player())) ) return 0;
    err = (mixed)env->CanVote( this_player() );
    if( !err ) return "This doesn't seem the proper place for it.";
    else return err;
}

mixed do_vote_for_str( string str ) {
    object env;

    if( !str || !(env = environment(this_player())) ) return 0;
        return (mixed)env->eventVote( this_player(), str );
}
