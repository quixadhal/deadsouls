/*    /verbs/rooms/cast.c
 *    from the Dead Soulsr2 Object Library
 *    cast OBJ in STR
 *    cast OBJ
 *    created by Descartes of Borg 951016
 */

#pragma save_binary

#include <lib.h>
#include "include/cast.h"

inherit LIB_DAEMON;

static void create() {
    daemon::create();
    SetNoClean(1);
    parse_init();
    parse_add_rule("cast", "OBJ");
}

mixed can_cast_obj(object ob) {
    object env;
    mixed err;

    if( !ob ) return 0;
    if( (err = (int)ob->CanCast(this_player())) != 1 )
      return (err || "You can't cast " + (string)ob->GetShort() + ".");
    if( !(env = environment(this_player())) ) return 0;
    err = (mixed)env->CanCast(this_player(), ob);
    if( err == 1 ) return 1;
    else if( !err ) return "It doesn't look like there is much fishing here.";
    else return err;
}

mixed do_cast_obj(object ob) { 
    object env;
    mixed err;

    if( !ob || !(env = environment(this_player())) ) return 0;
    if( err = (mixed)env->eventCast(this_player(), ob) ) return err;
}

string GetHelp(string str) {
    return "Syntax: <cast OBJ>\n\n"
      "Starts you fishing so long as OBJ is something you can fish with  "
      "and you are in a place that allows fishing.\n\n"
      "See also: fish, stop";
}
