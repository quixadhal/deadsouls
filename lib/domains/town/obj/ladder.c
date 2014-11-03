#include <lib.h>
#include <climb.h> // defines CLIMB_DOWN

//inherit LIB_DUMMY; // These do not show up in desc, but you can look at them
inherit LIB_ITEM;
inherit LIB_CLIMB; // This makes it climbable

int LadderClimb(){
    if(base_name(environment()) == "/domains/town/room/mansion_ext"){
        write("You climb up the ladder and into the mansion through the second-story window.");
        eventClimb(this_player(),CLIMB_UP,"/domains/town/room/mansion_uhall1");
        return 1;
    }
    if(base_name(environment()) == "/domains/amigara/room/passage1"){
        write("You climb up the ladder to the higher level of the cave.");
        eventClimb(this_player(),CLIMB_UP,"/domains/amigara/room/cave");
        return 1;
    }
    write("This ladder doesn't seem usable here.");
    return 0;
}

static void create() {
    ::create();
    SetKeyName("ladder");
    SetId("ladder");
    SetAdjectives(({"short","wood","wooden"}));
    SetShort("a ladder");
    SetLong("This is a standard wooden ladder, suitable for climbing.");
    SetClimb( (: LadderClimb :) , CLIMB_UP);
    SetCanClimbCarried(0);
    SetMass(100);
    SetBaseCost("silver", 10);
}

void init(){
    ::init();
}
