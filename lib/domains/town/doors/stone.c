#include <lib.h>

inherit LIB_DOOR;
//inherit LIB_READ;
//inherit LIB_LOOK;

static void create() {
    door::create();

    SetSide("north", ([ "id" : ({"door leading north", "door", "north door", "stone door"}),
                "short" : "a stone door leading north",
                "long" : "This is the south side of a stone door leading north. It is too heavy "
                "to be opened or closed by conventional means. There is an inscription on the door.",
                "lockable" : 0 ]) );

    SetSide("south", ([ "id" : ({"door leading south", "door", "south door", "stone door"}),
                "short" : "a stone door leading south",
                "long" : "This is the north side of a door leading south. It is too heavy "
                "to be opened or closed by conventional means.",
                "lockable" : 0 ]) );
    SetClosed(1);
    SetLocked(0);
}

varargs int eventOpen(object who, object tool) {
    object room;
    string tmp;

    room = environment(who);
    who->eventPrint("You fail to open " + GetShort(tmp) + ".");
    room->eventPrint(who->GetName() + " fails to open " + GetShort(tmp) + ".",
            who);
    return 0;
}

varargs mixed eventClose(object who){
    object room;
    string tmp;

    room = environment(who);
    who->eventPrint("You fail to close " + GetShort(tmp) + ".");
    room->eventPrint(who->GetName() + " fails to close " + GetShort(tmp) + ".",
            who);
    return 0;
}

void init(){
    ::init();
}
