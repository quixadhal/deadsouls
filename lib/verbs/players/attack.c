/*    /verbs/players/attack.c
 *    from the Dead Souls Object Library
 *    a very violent command
 *    created by Descartes of Borg 960512
 *    Version: @(#) attack.c 1.3@(#)
 *    Last Modified: 96/10/20
 */

#include <lib.h>
#include <position.h>
#include "include/attack.h"

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("attack");
    SetRules("LVS");
    SetErrorMessage("Attack whom?");
    SetHelp("Syntax: <attack LIVING>\n"
	    "        <attack all of LIVING>\n"
	    "        <attack all>\n\n"
	    "This command initiates combat with a living being or group "
	    "of living beings using any wielded weapons or your bare hands.  "
	    "Be very careful not to issue the \"attack all\" with other "
	    "players in the room or you will be guilty of attempted player "
	    "killing.\n\n"
	    "See also: wimpy");
}

mixed can_attack_liv(object target) {
    int pos = this_player()->GetPosition();

    if( (int)this_player()->GetParalyzed() ) {
	return "You cannot move!";
    }
    if( pos == POSITION_SITTING || pos == POSITION_LYING ) {
	return "You cannot attack in that position!";
    }
    if( (int)environment(this_player())->GetProperty("no attack") ) {
	message("environment", "A mystical force prevents your malice.",
		this_player());
	return 1;
    }
    return 1;
}

mixed do_attack_liv(object target) {
    return do_attack_lvs(({ target }));
}

mixed do_attack_lvs(mixed *targets) {
    object *obs;
    string tmp;

    obs = filter(targets, (: objectp :));
    if( !sizeof(obs) ) {
        mixed *ua;

	ua = unique_array(targets, (: $1 :));
	foreach(string *lines in ua) this_player()->eventPrint(lines[0]);
	return 1;
    }
    this_player()->SetAttack(obs);
    tmp = item_list(obs);
    obs->eventPrint((string)this_player()->GetName() + " attacks you!");
    environment(this_player())->eventPrint((string)this_player()->GetName() +
					   " attacks " + tmp + "!",
					   ({ this_player(), obs... }));
    this_player()->eventPrint("You advance towards " + tmp + ".");
    return 1;
}
