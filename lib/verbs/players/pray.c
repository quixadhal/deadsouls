/*    /verbs/players/pray.c
 *    From Dead Souls LPMud
 *    Created by Descartes of Borg 961101
 *    Version: @(#) pray.c 1.3@(#)
 *    Last modified: 96/11/03
 */

#include <lib.h>
#include <daemons.h>

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("pray");
    SetRules("for STR", "for STR against STR", "for STR against STR for LIV",
	     "for STR for OBJ");
    SetErrorMessage("Pray for what?");
    SetHelp("Syntax: <pray for PRAYER>\n"
	    "        <pray for PRAYER against SOMETHING>\n"
	    "        <pray for PRAYER against SOMETHING for SOMEONE>\n\n"
	    "Allows you to say a prayer to whatever deity you believe in.\n\n"
	    "See also: prayers");
}

mixed can_pray_for_str(string spell) {
    object tmp = SPELLS_D->GetSpell(spell);

    if( tmp ) {
	string verb = tmp->GetVerb();

	if( verb != "pray" ) {
	    return "That is not something you pray for.";
	}
    }
    return this_player()->CanCast(tmp);
}

mixed can_pray_for_str_against_str(string spell) {
    return can_pray_for_str(spell);
}

mixed can_pray_for_str_for_obj(string spell) {
    return can_pray_for_str(spell);
}

mixed can_pray_for_str_against_str_for_liv(string spell) {
    return can_pray_for_str(spell);
}

mixed do_pray_for_str(string spell) {
    return this_player()->eventPrepareCast(spell);
}

mixed do_pray_for_str_against_str(string spell, string ag) {
    return this_player()->eventPrepareCast(spell, ag);
}

mixed do_pray_for_str_for_obj(string spell, object ob) {
    return this_player()->eventPrepareCast(spell, ob);
}

mixed do_pray_for_str_against_str_for_liv(string spell, string ag, object tg) {
    return this_player()->eventPrepareCast(spell, ag, tg);
}
