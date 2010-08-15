#include <lib.h>
#include <props.h>

inherit LIB_BED;
inherit LIB_PRESS;
inherit LIB_MANIPULATE;

int MoveBed();
int PushBed();
static void create() {
    ::create();
    SetKeyName("king-sized bed");
    SetId( ({ "cot","bed" }) );
    SetAdjectives( ({ "large","king-sized","king-size","four-poster","four poster","big","very big"}) );
    SetShort("a large four-poster bed");
    SetLong("This is a very big bed, with a large, decorative wooden "
            "post at each corner. Though one might expect "
            "a canopy over such a bed, it is absent here.");
    SetMass(4000);
    SetMaxCarry(4000);
    SetBaseCost("silver",1500);
    SetMaxSitters(2);
    SetMaxLiers(1);
    SetPreventGet(1);
    SetManipulate( ([ "default" : (: MoveBed :) ]) );
    SetPress( ([ "default" : (: PushBed :) ]) );
}

int MoveBed(){
    send_messages("move", "$agent_name $agent_verb the bed.",
            this_player(), 0, environment(this_player()));
    environment(this_object())->OpenPassage();
    return 1;
}
int PushBed(){
    send_messages("move", "$agent_name $agent_verb the bed.",
            this_player(), 0, environment(this_player()));
    environment(this_object())->OpenPassage();
    return 1;
}
void init(){
    ::init();
}
