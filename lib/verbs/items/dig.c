/*    /verbs/items/dig.c
 *    From the Dead Soulsr2 Object Library
 *    Verb for digging things
 *    Created by Descartes of Borg 951030
 *    Version: @(#) dig.c 1.2@(#)
 *    Last modified: 97/01/01
 */

#include <lib.h>
#include <rounds.h>
#include <position.h>

inherit LIB_VERB;

static void eventDig(object who, object tool, object what, object check) {
    if( !who || check != environment(who) ) {
	return;
    }
    if( !tool || (environment(tool) != who) ) {
	who->eventPrint("You have lost your equipment.");
	return;
    }
    tool->eventDigWith(who, what);
}

varargs static void eventPrepareDig(object who, object tool, object what) {
    function f = (: eventDig($(who), $(tool), $(what), environment($(who))) :);
    
    if( (int)this_player()->GetInCombat() ) {
	send_messages("start", "$agent_name $agent_verb to dig with " +
		      tool->GetShort() + ".", who, 0, environment(who));
	who->SetAttack(0, f, ROUND_OTHER);
    }
    else {
	evaluate(f);
    }
}

static void create() {
    verb::create();
    SetVerb("dig");
    SetRules("with OBJ", "STR with OBJ", "OBJ with OBJ");
    SetErrorMessage("Dig with what?  Is there anything in particular to dig?");
    SetHelp("Syntax: <dig with TOOL>\n"
	    "        <dig THING with TOOL>\n\n"
	    "Allows you to dig with digging tools.  Sometimes you may "
	    "be required to specify what it is you wish to dig.  In "
	    "that case, you can use the thing field to specify what it is "
	    "you wish to dig.\n\n"
	    "See also: bury");
}

mixed can_dig_with_obj() {
    if( this_player()->GetParalyzed() ) {
	return "You cannot do anything!";
    }
    if( this_player()->GetPosition() != POSITION_STANDING ) {
	return "Dig when you are not standing?";
    }
    return 1;
}

mixed can_dig_str_with_obj() {
    return can_dig_with_obj();
}

mixed can_dig_obj_with_obj() {
    return can_dig_with_obj();
}

mixed do_dig_with_obj(object ob) {
    eventPrepareDig(this_player(), ob);
    return 1;
}

mixed do_dig_str_with_obj(string str, object ob) {
    if( remove_article(lower_case(str)) != "hole" ) {
	return 0;
    }
    return do_dig_with_obj(ob);
}

mixed do_dig_obj_with_obj(object what, object ob, string id) {
    eventPrepareDig(this_player(), ob, what);
    return 1;
}
