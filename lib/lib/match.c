/*    /lib/match.c
 *    from the Dead Souls V Object Library
 *    an object which can be self-induced to burn
 *    created by Descartes of Borg 960512
 */

#include <lib.h>
#include "include/match.h"

inherit LIB_TORCH;

private int StrikeChance = 50;

mixed direct_strike_obj() {
    if( environment() != this_player() ) return "#You don't have that!";
    if( GetBurning() ) return "It is already lit!";
    return 1;
}

mixed eventStrike(object who) {
    if( !GetFuelAmount() || StrikeChance < random(100) || !eventBurn(who) ) {
	who->eventPrint("You strike " + GetShort() + ", but nothing happens.");
	environment(who)->eventPrint((string)who->GetName() + " stikes " +
				     GetShort() + ", but nothing happens.",
				     who);
	return 1;
    }
    who->eventPrint("You strike " + GetShort() + " and it ignites!");
    environment(who)->eventPrint((string)who->GetName() + " strikes " +
				 GetShort() + " and it ignites!", who);
    return 1;
}

int GetStrikeChance() {
    return StrikeChance;
}

static int SetStrikeChance(int x) {
    return (StrikeChance = x);
}
