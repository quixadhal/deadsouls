/*    /verbs/items/bury.c
 *    From the Dead Soulsr2 Object Library
 *    Buries objects where no one can find them
 *    Created by Descartes of Borg 951030
 */

#include <lib.h>
#include <rounds.h>
#include <position.h>

inherit LIB_VERB;

static void eventBury(object who, object tool, object what, object check) {
    if( !who || environment(who) != check ) {
	return;
    }
    if( !tool || (environment(tool) != who) ) {
	who->eventPrint("You have lost your digging equipment.");
	return;
    }
    tool->eventBuryWith(who, what);
}

varargs static void eventPrepareBury(object who, object tool, object what) {
    function f = (: eventBury($(who), $(tool), $(what),environment($(who))) :);
    
    if( who->GetInCombat() ) {
	send_messages("start", "$agent_name $agent_verb to bury " +
		      "with " + tool->GetShort() + ".", who, 0,
		      environment(who));
	who->SetAttack(0, f, ROUND_OTHER);
    }
    else {
	evaluate(f);
    }
}

static void create() {
    verb::create();
    SetVerb("bury");
    SetErrorMessage("Bury what with what?");
    SetRules("OBJ with OBJ", "STR with OBJ");
    SetHelp("Syntax: <bury ITEM with TOOL>\n\n"
	    "Allows you to use a digging tool to bury things.\n\n"
	    "See also: dig");
}

mixed can_bury_str_with_obj() {
    if( this_player()->GetParalyzed() ) {
	return "You cannot do anything!";
    }
    if( this_player()->GetPosition() != POSITION_STANDING ) {
	return "You can only bury things while standing!";
    }
    return 1;
}

mixed can_bury_obj_with_obj() {
    return can_bury_str_with_obj();
}

mixed do_bury_str_with_obj(string str, object tool) {
    eventPrepareBury(this_player(), tool);
    return 1;
}

mixed do_bury_obj_with_obj(object what, object tool) {
    eventPrepareBury(this_player(), tool, what);
    return 1;
}
