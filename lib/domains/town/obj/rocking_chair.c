#include <lib.h>

inherit LIB_CHAIR;
inherit LIB_ROCK;

static void create() {
    chair::create();
    SetKeyName("chair");
    SetAdjectives( ({"simple", "wood", "wooden", "rocking", "classic"}) );
    SetId("chair");
    SetShort("a wooden rocking chair");
    SetLong("A simple wooden rocking chair, fashioned in a classic, non-nonsense style common in the rural parts of town.");
    SetMass(100);
    SetDollarCost(15);
    SetMaxSitters(1);
    SetPreventGet("The chair does not budge.");
}

void init(){
    ::init();
}

varargs mixed eventRock(object who){
    if(member_array(who, GetSitters()) != -1){
        write("You rock back and forth in your chair.");
        say(this_player()->GetCapName()+" rocks back and forth "+
                "on "+possessive(who)+" rocking chair.");
        return 1;
    }
    else {
        write("You're not sitting in it!");
        return 1;
    }
}
