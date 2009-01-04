#include <lib.h>

inherit LIB_DOOR;

static void create() {
    door::create();

    SetSide("east", ([ "id" : ({"pressure door leading east", "door", "pressure door", "east door"}),
                "short" : "a pressure door leading east",
                "long" : "This is the west side of a pressure door leading east. It is huge and heavy and designed to withstand and deflect great water pressure.",
                "lockable" : 1 ]) );

    SetSide("west", ([ "id" : ({"pressure door leading west", "door", "pressure door", "west door"}),
                "short" : "a pressure door leading west",
                "long" : "This is the east side of a pressure door leading west. It is huge and heavy and designed to withstand and deflect great water pressure.",
                "lockable" : 1 ]) );

    SetClosed(1);
    SetLocked(1);
    SetKeys("east", ({ "lab key" }));
    SetKeys("west", ({ "lab key" }));
}

varargs mixed CanOpen(object ob, string id){
    mixed ret = ::CanOpen(ob, id); 
    if(!ret || !intp(ret)) return ret;
    if(ob->GetStatLevel("strength") > 49){
        return 1;
    }
    else {
        write("You lack sufficient physical strength!");
        return 0;
    }
}

varargs mixed CanClose(object who, string id){
    mixed ret = ::CanClose(who, id);
    if(!ret || !intp(ret)) return ret;
    if(who->GetStatLevel("strength") > 49){
        return 1;
    }
    else {
        write("You lack sufficient physical strength!");
        return 0;
    }
}

void init(){
    ::init();
}
