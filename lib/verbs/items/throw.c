/*    /verbs/items/throw.c
 *    from the NIghtmare IVr2 Object Library
 *    created by Descartes of Borg 951029
 *    Version: @(#) throw.c 1.3@(#)
 *    Last modified: 96/10/20
 */

#include <lib.h>
#include <rounds.h>

inherit LIB_VERB;

mixed do_throw_obj_word_obj(object what, string word, object where);
void eventThrow(object who, object what, object where);

static void create() {
    verb::create();
    SetVerb("throw");
    SetSynonyms("toss");
    SetRules("OBJ", "OBJ at OBJ", "OBJ into OBJ");
    SetErrorMessage("Throw what?");
    SetHelp("Syntax: <throw OBJ>\n"
	    "        <throw OBJ at OBJ>\n"
	    "        <throw OBJ into OBJ>\n\n"
	    "Allows you to throw an object.  Some object you may also "
	    "use offensively by throwing them.  Other places may simply "
	    "allow you to throw objects into them, say like throwing "
	    "equipment down to a friend in a chasm.\n"
	    "Note that throwing a weapon at someone will initiate "
	    "combat.\n\n"
	    "Synonyms: toss");
}

mixed can_throw_obj() {
    if( this_player()->GetParalyzed() ) {
	return "You cannot do anything.";
    }
    return 1;
}

mixed can_throw_obj_word_obj() {
    return can_throw_obj();
}

mixed do_throw_obj(object ob) {
    return do_throw_obj_word_obj(ob, 0, 0);
}

mixed do_throw_obj_word_obj(object what, string word, object where) {
    object enemy;

    if( where && living(where) && (int)what->GetClass() > 1 ) {
	enemy = where;
    }
    else {
	enemy = 0;
    }
    if( this_player()->GetInCombat() || enemy ) {
	this_player()->eventPrint("You prepare to throw " +
				  (string)what->GetShort() + ".");
	this_player()->SetAttack(enemy, (: eventThrow, this_player(), what,
					 where :), (enemy ? ROUND_WEAPON :
						    ROUND_OTHER));
	return 1;
    }
    eventThrow(this_player(), what, where);
    return 1;
}

void eventThrow(object who, object what, object where) {
    mixed tmp;

    if( !who ) {
	return;
    }
    if( !what || environment(what) != who ) {
	who->eventPrint("You no longer have anything to throw.");
	return;
    }
    what->eventThrow(who, where);
}
