#include <lib.h>

inherit LIB_DUMMY; // These do not show up in desc, but you can look at them
inherit LIB_PRESS; // Makes the item pressable

int openDoor(object who) {
    object car;
    send_messages("press", "$agent_name $agent_verb the button.",
            who, 0, environment(who));
    car=load_object("/domains/town/room/elevator");
    car->CallMe(1);

}

static void create() {
    dummy::create();
    SetKeyName("button");
    SetId(({"button","button on the wall", "button next to the elevator", "button next to elevator","button next to an elevator" }));
    SetAdjectives("call", "elevator");
    SetShort("a button");
    SetLong("It is a button that you could probably press.");
    SetPress((: openDoor :));
}
