/*    /verbs/players/withdraw.c
 *    from the Dead Souls  Object Library
 *    Allows players to withdraw from elections
 *    created by Kalinash 961021
 *    Version: @(#) withdraw.c 1.1@(#) Last modified: 96/10/21
 */

#pragma save_binary

#include <lib.h>
#include "include/withdraw.h"

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("withdraw");
    SetRules("");
    SetHelp("Syntax: <withdraw>\n"
            "Allows you to withdraw from the elections");
}

mixed can_withdraw() {
    object env;
    mixed  err;

    if( !(env = environment(this_player())) ) return 0;
    err = (mixed)env->CanWithdraw( this_player() );
    if( !err ) return "This doesn't seem the proper place for it.";
    else return err;
}

mixed do_withdraw() {
    object env;

    if( !(env = environment(this_player())) ) return 0;
        return (mixed)env->eventWithdraw( this_player() );
}
