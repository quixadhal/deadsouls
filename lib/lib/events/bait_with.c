/*    /lib/events/bait_with.c
 *    From the Dead Souls V Object Library
 *    Handles putting bait on fishing devices
 *    Created by Blitz@Dead Souls 960116
 *    Version: @(#) bait_with.c 1.2@(#)
 *    Last modified: 97/01/03
 */
 
#include <lib.h>
 
private int BaitStrength = 0;

// abstract methods
string GetShort();
static int Destruct();
// end abstract methods

int GetBaitStrength() {
    return BaitStrength;
}
 
int SetBaitStrength(int x) {
    return (BaitStrength = x);
}
 
mixed eventBait(object who, object pole) {
    if( !(who && pole) ) {
	return 0;
    }
    if( !pole->eventBait(who, this_object()) ) {
        who->eventPrint(capitalize(pole->GetDefiniteShort()) +
                        "remains unbaited.");
        return 1;
    }
    send_messages("bait", "$agent_name $agent_verb $target_name with " +
		  GetShort() + ".", who, pole, environment(who));
    Destruct();
    return 1;
}
 
mixed indirect_bait_obj_with_obj() {
    if( environment() != this_player() ) {
	return "#You don't have that bait!";
    }
    return 1;
}
