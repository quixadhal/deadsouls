/*    /lib/sit.c
 *    From the Dead Souls V Object Library
 *    Handles people sitting down in it
 *    Created by Descartes of Borg 961221
 *    Version: @(#) sit.c 1.1@(#)
 *    Last modified: 96/12/21
 */

#include <position.h>

private int          MaxSitters = 1;
private object array Sitters    = ({});

int GetMaxSitters() {
    return MaxSitters;
}

static int SetMaxSitters(int x) {
    return (MaxSitters = x);
}

object array GetSitters() {
    return Sitters;
}

mixed eventReceiveSit(object who) {
    Sitters = ({ Sitters..., who });
    return 1;
}

mixed eventReleaseStand(object who) {
    Sitters -= ({ who });
    return 1;
}

mixed direct_sit_word_obj() {
    Sitters = filter(Sitters, (: $1 && $1->GetPosition()==POSITION_SITTING :));
    if( sizeof(Sitters) >= MaxSitters ) {
	return "There is no room to sit there.";
    }
    return 1;
}

mixed direct_sit_down_word_obj() {
    return direct_sit_word_obj();
}
