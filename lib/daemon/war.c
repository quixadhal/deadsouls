/*    /daemon/war.c
 *    From Dead Souls LPMud
 *    A daemon to manage who can kill whom
 *    Created by Descartes of Borg 961117
 *    Version: %A%
 *    Last modified: %D%
 */

#include <lib.h>

inherit LIB_DAEMON;

private int LastWar = 0;
private class war War = 0;

static void create() {
    daemon::create();
    SetNoClean(1);
}

varargs int GetAtWar(object agent, object target) {
    string name;
    
    if( !War ) {
	return 0;
    }
    if( !agent ) {
	return 1;
    }
    name = agent->GetKeyName();
    if( member_array(name, War->aggressor->members) == -1 ) {
	if( member_array(name, War->enemy->members) == -1 ) {
	    return 0;
	}
    }
    if( !target ) {
	return 1;
    }
    name = target->GetKeyName();
    if( member_array(name, War->aggressor->members) == -1 ) {
	if( member_array(name, War->enemy->members) == -1 ) {
	    return 0;
	}
    }
    return 1;
}

mixed eventDeclareWar(object who, string victim, string reason) {
    if( GetAtWar() ) {
	return "There is already a war raging between " +
	    item_list(map(GetEnemies(), (: capitalize :))) + ".";
    }
    if( !leaderp(who) ) {
	return "You do not have the power to declare war.";
    }
    if( time() - WAR_INTERVAL < LastWar ) {
	return "Let the dust settle from the last war!";
    }
    obs = filter(users(), function(object ob, string victim) {
	                      if( !leaderp(ob) ) {
				  return 0;
			      }
			      return ob->ClassMember(victim);
                          }, victim);
    obs -= ({ who });
    if( !sizeof(obs) ) {
	return "The enemy must have a leader online for a declaration of war.";
    }
    War = new(class war);
    War->start = time();
    War->reason = reason;
    War->aggressor = new(class team);
    War->aggressor->name = who->GetClass(); // multi-class???
    War->aggressor->leader = who->GetKeyName();
    War->aggressor->points = 0;
    War->aggressor->war_chest = 0;
    War->aggressor->members = ({ who->GetKeyName() });
    War->enemy = new(class team);
    War->enemy->name = victim;
    War->enemy->leader = enemy->GetKeyName();
    War->enemy->points = 0;
    War->enemy->war_chest = 0;
    War->enemy->members = ({ enemy->GetKeyName() });
    users()->eventPrint("%^BOLD%^GREEN%^" + who->GetCapName() +
			" has declared war on " + pluralize(victim) + "!");
}
