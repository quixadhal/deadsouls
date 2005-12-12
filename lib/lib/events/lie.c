#include <position.h>

private int          MaxLiers = 1;
private object array Liers    = ({});

int GetMaxLiers() {
    return MaxLiers;
}

static int SetMaxLiers(int x) {
    return (MaxLiers = x);
}

object array GetLiers() {
    return Liers;
}

mixed eventReceiveLay(object who) {
    Liers = ({ Liers..., who });
    return 1;
}

mixed eventReleaseStand(object who) {
    Liers -= ({ who });
    return 1;
}

mixed direct_lie_word_obj() {
    Liers = filter(Liers, (: $1 && $1->GetPosition()==POSITION_LYING :));
    if( sizeof(Liers) >= MaxLiers ) {
	return "There is no room to lie there.";
    }
    if(environment() != environment(this_player())) {
	return "That's not available for sitting right now.";
    }
    return 1;
}

mixed direct_lie_down_word_obj() {
    return direct_lie_word_obj();
}
