#include <lib.h>
#include <position.h>
#include <rounds.h>

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("sink");
    SetRules("", "down");
    SetErrorMessage("Sink down?");
    SetHelp("Syntax: sink [down]\n"
            "Allows you to stop swimming, or otherwise attempt to "
            "sink in your current medium.\n"
            "See also: sit, lie, stand, swim, fly");
}

mixed can_sink_down() {
    if( this_player()->GetParalyzed() ) {
        return "You cannot do anything!";
    }
    if(this_player()->CanSink()){
        return "You can't sink here.";
    }
    if( this_player()->GetPosition() == POSITION_FLOATING ) {
        return "You are already floating!";
    }
    return 1;
}

mixed can_sink(){
    return can_sink_down();
}

mixed do_sink_down(){
    this_player()->SetPosition(POSITION_FLOATING);
    return this_player()->eventSink();
}

mixed do_sink() {
    return do_sink_down();
}
