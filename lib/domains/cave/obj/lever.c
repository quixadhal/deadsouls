#include <lib.h>
#include <medium.h>

inherit LIB_DUMMY; // These do not show up in desc, but you can look at them
inherit LIB_PULL; // Makes the item pullable
inherit LIB_PRESS; // Makes the item pressable

private static int Sprung = 0;
object chiefroom = load_object("/domains/cave/room/chieftain");

int GetSprung(){
    if(Sprung) return 1;
    return 0;
}

int openDoor(object who) {
    if(GetSprung()){
        write("The lever is already in the pulled position.");
    }
    else {
        object *inv = ( all_inventory(chiefroom) - ({ this_object() }) );
        write("You pull the lever, and the floor drops out!");
        say(this_player()->GetName()+" pulls the lever, and the floor drops out!");
        tell_room("/domains/cave/room/cavetroll","The ceiling opens up.");
        chiefroom->SetMedium(MEDIUM_AIR);
        chiefroom->AddExit("down","/domains/cave/room/cavetroll");
        inv->eventCheckEnvironment();
        filter(inv,(: !living($1) :))->eventFall();
    }
    Sprung = 1;
    return 1;
}

int closeDoor(object who){
    if(!GetSprung()){
        write("The lever is already in the pushed position.");
    }
    else {
        write("You push the lever, and the floor closes.");
        say(this_player()->GetName()+" pushes the lever, and the floor closes.");
        tell_room("/domains/cave/room/cavetroll","The ceiling closes.");
        chiefroom->SetMedium(MEDIUM_LAND);
        chiefroom->RemoveExit("down");
    }
    Sprung = 0;
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
