//	/bin/adm/_allow.c
//	from the Dead Souls mudlib
//	lets in selected players from sites on register
//	created by Shadowwolf@Dead Souls?

#include <lib.h>
#include <privs.h>
#include <daemons.h>

inherit LIB_DAEMON;

int cmd(string str) {
    string who, email;
    mixed res;

    if(!archp(previous_object())) return 0;
    if(!member_group(previous_object(), PRIV_SECURE)) {
    if(!member_group(previous_object(), "LAW")) {
	notify_fail("Only law may allow new players in the game.\n");
	return 0;
    }
    }
    if(!str) return 0;
    if(sscanf(lower_case(str), "%s %s", who, email) != 2) {
	notify_fail("Syntax: <allow [who] [email]>\n");
	return 0;
    }
    if(user_exists(who)) {
	notify_fail("A character by that name already exists.\n");
	return 0;
    }
    write(capitalize(who)+" is now allowed to create a character.\n");
    if(res = catch(call_other(BANISH_D, "allow_name", who)))
      write("Error in letting in "+who+": "+res+"\n");
    log_file("watch/players", capitalize(who)+": "+email+" ("+ctime(time())+")\n");
    return 1;
}

void help() {
    write("Syntax: <allow [who] [email] >\n\n");
    write("Members of law may allow people from sites on register to create\n");
    write("new characters with this command.  [who] is the character name,\n");
    write("[email] is the player's real life email address.\n");
    write("See also: noallow, wholetin, whobanished, whoregistered, whowatched\n");
}
