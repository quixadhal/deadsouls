/*    /daemon/combat.c
 *    From the Dead Souls V Object Library
 *    Single point for storing the large arrays where combat messages are
 *    defined.
 *    Created by Descartes of Borg 961117
 *    Version: %A%
 *    Last modified: %D%
 */

#include <lib.h>

inherit LIB_DAEMON;

private mixed array BLADES = ({
    // no damage   
    ({ ({ ({ "take" }), "$target_verb the cut from $limb without a "
	                "flinch." }) }),
    // damage < 5  
    ({ ({ ({ "prick" }), "$agent_verb $target_name lightly in the $limb with "
	                 "$using." }),
       ({ ({ "prick" }), "$agent_verb $target_name superficially in the $limb "
	                 "with $using." }),
       ({ ({ "prick" }), "just barely $agent_verb $target_name in the $limb "
	                 "with $using." }) }),
    // damage < 10
    ({ ({ ({ "prick" }), "$agent_verb $target_name decently in the $limb with "
	                 "$using." }),
       ({ ({ "prick" }), "$agent_verb $target_name annoyingly in the $limb "
	                 "with $using." }),
       ({ ({ "prick" }), "solidly $agent_verb $target_name in the $limb "
	                 "with $using." }) }),
    // damage < 15
    ({ ({ ({ "scratch" }), "$agent_verb $target_name mildly in the $limb "
	                   "with $using."}),
       ({ ({ "scratch" }), "barely $agent_verb $target_name in the $limb "
	                   "with $using." }),
       ({ ({ "scratch" }), "$agent_verb $target_name in the $limb with "
	                   "$using." }) }),
    // damage < 20
    ({ ({ ({ "scratch" }), "$agent_verb $target_name nastily in the $limb "
	                   "with $using." }),
       ({ ({ "scratch" }), "$agent_verb $target_name wickedly in the $limb "
	                   "with $using." }) }),
    // damage < 25
    ({ ({ ({ "jab" }), "$agent_verb $target_name in the $limb with $using." }),
       ({ ({ "jab" }), "quickly $agent_verb $target_name in the $limb with "
	               "$using." }) }),
    // damage < 30
    ({ ({ ({ "jab" }), "$agent_verb $target_name meanly in the $limb with "
	               "$using." }),
       ({ ({ "jab" }), "$agent_verb $target_name solidly in the $limb with "
	               "$using." })
    // damage < 50
    ({ ({ ({ "cut" }), "$agent_verb $target_name pain
       

private mixed array MOVES = ({
    // baby moves
    ({ ({ ({ "shake" }), "$agent_name $agent_verb oddly and " }),
       ({ ({ "dance" }), "$agent_name $agent_verb and " }),
       ({ ({ "shout" }), "$agent_name $agent_verb profanities and " }),
       ({ ({ "sidestep" }), "$agent_name $agent_verb weakly and " }),
       ({ ({ "blow" }), "$agent_name $agent_verb snot and " }),
       ({ ({ "swing" }), "$agent_name $agent_verb blindly and " }),
       ({ ({ "bellow" }), "$agent_name $agent_verb loudly and " }),
       ({ ({ "see" }), "$agent_name $agent_verb an opening and " }),
       ({ ({ "stumble" }), "$agent_name $agent_verb fortuitously and " }) }),
    // better moves
    ({ ({ ({ "grunt" }), "$agent_name $agent_verb angrily and " }),
       ({ ({ "scream" }), "$agent_name $agent_verb \"Die!\", and " }),
       ({ ({ "spit", "blow" }), "$agent_name $agent_verb, $agent_verb, and "}),
       ({ ({ "slobber" }), "$agent_name $agent_verb evilly and " }),
       ({ ({ "duck" }), "$agent_name $agent_verb and " }),
       ({ ({ "rant" }), "$agent_name $agent_verb, \"Aaaarrrrggghhh!\" and " }),
       ({ ({"glare"}), "$agent_name $agent_verb at $target_name evilly and "}),
       ({ ({ "growl" }), "$agent_name $agent_verb menacingly and " }) }),
    // awesome moves
    ({ ({ ({ "rally" }), "$agent_name $agent_verb with determination and " }),
       ({ ({ "lunge" }), "$agent_name $agent_verb quickly and " }),
       ({ ({ "twirl" }), "$agent_name $agent_verb with finesse and " }),
       ({ ({ "execute" }), "$agent_name $agent_verb a diving roll and " }),
       ({ ({ "go" }), "$agent_name $agent_verb completely berserk and " }),
       ({ ({ "whirl" }), "$agent_name $agent_verb blurringly and " }) }) });

mixed array GetMessages(string type) {
    switch(type) {
    case "blade":       return BLADES;
    case "blunt":       return BLUNTS;
    case "knife":       return KNIVES;
    case "projectile":  return PROJECTILES;
    default:            return BLUNTS;
    }
}

mixed array GetMove(int amt) {
    if( amt < 2 ) {
	return 0;
    }
    amt = random(amt);
    if( amt < 4 ) {
	list = MOVES[0];
    }
    else if( amt < 7 ) {
	list = MOVES[1];
    }
    else {
	list = MOVES[2];
    }
    return list[random(sizeof(list))];
}
