#include <lib.h>

inherit LIB_DUMMY; // These do not show up in desc, but you can look at them
inherit LIB_PULL; // Makes the item pullable
inherit LIB_PRESS; // Makes the item pressable

int openDoor(object who) {
    object door1 = load_object("/domains/town/doors/welldoor1");
    object door2 = load_object("/domains/town/doors/welldoor2");
    if(!door1->GetClosed()){
        write("The lever is already in the pulled position.");
    }
    else {
        write("You pull the lever, and the west door opens.");
        say(this_player()->GetName()+" pulls the lever, and the west door opens.");
        tell_room("/domains/town/room/well2","The east door opens, and the west door closes.");
        tell_room("/domains/town/room/well3","The east door closes.");
        door1->SetClosed(0);
        door2->SetClosed(1);
    }
    return 1;
}

int closeDoor(object who){
    object door1 = load_object("/domains/town/doors/welldoor1");
    object door2 = load_object("/domains/town/doors/welldoor2");
    if(door1->GetClosed()){
        write("The lever is already in the pushed position.");
    }
    else {
        write("You push the lever, and the west door closes.");
        say(this_player()->GetName()+" pushes the lever, and the west door closes.");
        tell_room("/domains/town/room/well2","The east door closes, and the west door opens.");
        tell_room("/domains/town/room/well3","The east door opens.");
        door1->SetClosed(1);
        door2->SetClosed(0);
    }
    return 1;
}

static void create() {
    dummy::create();
    SetKeyName("lever");
    SetId(({"lever","lever on the wall" }));
    SetAdjectives("wooden");
    SetShort("a lever");
    SetLong("It is a lever.");
    SetPress((: closeDoor :));
    SetPull((: openDoor :));
}
