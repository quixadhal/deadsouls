/*    /lib/events/bait.c
 *    From the Dead Souls V Object Library
 *    Handles the bait event
 *    Created by Descartes of Borg 970103
 *    Version: @(#) bait.c 1.1@(#)
 *    Last modified: 97/01/03
 */

private int Bait = 0;

// abstract methods
string GetDefiniteShort();
// end abstract methods

int AddBait(int x) {
    return (Bait += x);
}
 
int GetBait() {
    return Bait;
}
 
int SetBait(int x) {
    return (Bait = x);
}

string array GetSave() {
    return ({ "Bait" });
}

mixed CanBait(object who) {
    if( GetBait() ) {
	return "There is already bait on " + GetDefiniteShort() + ".";
    }
    return 1;
}
    
int eventBait(object who, object bait) {
    AddBait(bait->GetBaitStrength());
    return 1;
}
 
mixed direct_bait_obj_with_obj() { 
    if( environment() != this_player() ) {
	return "#You don't have that!";
    }
    return CanBait(this_player());
}

