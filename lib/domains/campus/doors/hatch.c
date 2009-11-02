#include <lib.h>

inherit LIB_DOOR;

static void create() {
    door::create();
    SetSide("down", ([ "id" : ({"service hatch", "hatch"}),
                "short" : "a hatch",
                "long" : "This is a service hatch.",
                "lockable" : 0 ]) );
    SetSide("up", ([ "id" : ({"service hatch", "hatch"}),
                "short" : "a hatch",
                "long" : "This is a service hatch.",
                "lockable" : 0 ]) );
    SetClosed(1);
}

mixed CanOpen(object ob){
    if(base_name(environment(ob)) == "/domains/campus/room/maintenance"){
        return 1;
    }
    else return "There is no handle on this side.";
}
