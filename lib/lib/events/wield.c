/*    /lib/events/wield.c
 *    From the Dead Souls V Object Library
 *    Handles the wielding of things
 *    Created by Descartes of Borg 970101
 *    Version: @(#) wield.c 1.1@(#)
 *    Last modified: 97/01/01
 */

// abstract methods
int GetHands();
mixed CanEquip(object who, string array limbs);
mixed CanUnequip(object who);
// end abstract methods

mixed direct_unwield_obj() {
    if( environment() != this_player() ) {
	return "#You don't have that!";
    }
    return CanUnequip(this_player());
}
 
mixed direct_wield_obj() {
    int hands = GetHands();
    string array limbs;
 
    if( environment() != this_player() ) {
	return "#You don't have that!";
    }
    limbs = (this_player()->GetWieldingLimbs() || ({}));
    limbs = filter(limbs, (: !this_player()->GetWielded($1) :));
    if( !sizeof(limbs) ) {
	return "You have nowhere to wield it!";
    }
    if( sizeof(limbs) > hands ) {
	limbs = limbs[0..(hands-1)];
    }
    return CanEquip(this_player(), limbs);
}
 
mixed direct_wield_obj_word_str(object target, string wrd, string limb) {
    if( environment() != this_player() ) {
	return "#You don't have that!";
    }
    return CanEquip(this_player(), explode_list(limb));
}
