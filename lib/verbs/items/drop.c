/*    /verbs/items/drop.c
 *    from the Dead Souls V Object Library
 *    drop OBJ
 *    drop OBS
 *    drop WRD WRD
 *    created by Descartes of Borg 960113
 */

#pragma save_binary

#include <lib.h>
#include "include/drop.h"

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("drop");
    SetSynonyms("put down");
    SetRules("OBS", "WRD WRD");
    SetErrorMessage("Drop what?");
    SetHelp("Syntax: <drop ITEM>\n"
	    "        <drop all>\n"
	    "        <drop all ITEM TYPE>\n"
	    "        <drop AMOUNT CURRENCY>\n\n"
	    "Allows you to drop something you have, or to drop an amount of "
	    "some currency you have on you.\n\n"
	    "See also: get, put");
}

mixed can_drop_obj(object ob) { return 1; }

mixed can_drop_wrd_wrd(string num, string curr) {
    int amt;
    
    if( !num || !curr ) return 0;
    if( (amt = to_int(num)) < 1 ) return "You cannot do that!";
    if( (int)this_player()->GetCurrency(curr) < amt )
      return "You don't have that much " + curr + ".";
    return 1;
}

mixed do_drop_obj(object ob) {
    return (mixed)ob->eventDrop(this_player());
}

mixed do_drop_obs(mixed *res) {
    object *obs;
    mixed tmp;

    if( !sizeof(res) ) {
        this_player()->eventPrint("You don't have any to drop!");
        return 1;
    }
    obs = filter(res, (: objectp :));
    if( !sizeof(obs) ) {
        mixed *ua;

	ua = unique_array(res, (: $1 :));
	foreach(string *list in ua) this_player()->eventPrint(list[0]);
	return 1;
    }
    foreach(object ob in obs) 
      if( (tmp = (mixed)ob->eventDrop(this_player())) != 1 ) {
	  if( stringp(tmp) ) this_player()->eventPrint(tmp);
	  else this_player()->eventPrint("You cannot drop " +
					 (string)ob->GetShort() + ".");
      }
    return 1;
}

mixed do_drop_wrd_wrd(string num, string curr) {
    object pile, env;
    int amt;

    amt = to_int(num);
    env = environment(this_player());
    pile = new(LIB_PILE);
    pile->SetPile(curr, amt);
    if( !((int)pile->eventMove(env)) ||
       (int)this_player()->AddCurrency(curr, -amt) == -1 ) {
	this_player()->eventPrint("Something prevents your action.");
	pile->eventDestruct();
	return 1;
    }
    this_player()->eventPrint("You drop " + amt + " " + curr + ".");
    environment(this_player())->eventPrint((string)this_player()->GetName() +
					   " drops some " + curr + ".",
					   this_player());
    return 1;
}
