/*    /lib/steal.c
 *    from the Dead Souls V Object Library
 *    handles the stealing of objects
 *    created by Descartes of Borg 960210
 */

#include "include/steal.h"

static private mixed PreventSteal;

static void create() {
    PreventSteal = 0;
}

mixed direct_steal_obj_from_liv() { return CanSteal(this_player()); }

mixed CanSteal(object who) {
    if( GetInvis(who) ) return 0;
    if( PreventSteal ) {
        if( PreventSteal == 1 ) return "You can't steal that!";
	if( stringp(PreventSteal) ) return PreventSteal;
	if( objectp(PreventSteal) ) {
	    if( PreventSteal == who )
	      return "Mystical forces prevent your thievery.";
	    return 1;
	}
	return evaluate(PreventSteal, who);
    }
    return 1;
}

mixed eventSteal(object who) {
    return eventMove(who);
}

mixed SetPreventSteal(mixed val) { return (PreventSteal = val); }

mixed GetPreventSteal() { return PreventSteal; }
