#include <lib.h>

inherit LIB_DUMMY; // These do not show up in desc, but you can look at them
inherit LIB_PRESS; // Makes the item pressable

int openDoor(object who) {
    object car;
    send_messages("press", "$agent_name $agent_verb the button.",
            who, 0, environment(who));
    car=load_object("/domains/town/room/elevator");
    car->SetDoor(1);
    car->CallMe(1);

}

static void create() {
    dummy::create();
    SetKeyName("button 1");
    SetId("button","one","1");
    SetAdjectives("elevator","button","one","1");
    SetShort("a button");
    SetLong("It is a button that you could probably press. The numeral \"1\" " 
            "is printed on it.");
    SetPress((: openDoor :));
}
