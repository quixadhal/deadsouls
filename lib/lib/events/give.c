/*    /lib/events/give.c
 *    From the Dead Souls V Object Library
 *    Give event handler
 *    Created by Descartes of Borg 970101
 *    Version: @(#) give.c 1.1@(#)
 *    Last modified: 97/01/01
 */

// abstract methods
mixed CanDrop(object who);
string GetDefiniteShort();
int GetMass();
// end abstract methods

mixed indirect_give_liv_obj(object target) {
    mixed tmp;

    if( environment() != this_player() ) {
	return "#You cannot give what is not yours.";
    }
    tmp = CanDrop(this_player());
    if( tmp != 1 ) {
	return (tmp || "You can't drop " + GetDefiniteShort() + ".");
    }
    tmp = target->CanCarry(GetMass());
    if( tmp != 1 ) {
	return (tmp || capitalize(GetDefiniteShort()) + " is too heavy.");
    }
    return 1;
}
 
mixed direct_give_obj_to_liv() {
    mixed tmp;
 
    if( environment() != this_player() ) {
	return "#You cannot give what is not yours.";
    }
    tmp = CanDrop(this_player());
    if( tmp != 1 ) {
	return (tmp || "You can't drop " + GetDefiniteShort() + ".");
    }
    return 1;
}

