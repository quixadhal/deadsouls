#include <lib.h>

inherit LIB_DOOR;

static void create() {
    door::create();

    SetSide("down", ([ "id" : ({"grate leading down", "grate", "down grate"}),
                "short" : "a grate leading down",
                "long" : "This is the up side of a grate leading down. It looks very heavy.",
                "lockable" : 0 ]) );

    SetSide("up", ([ "id" : ({"grate leading up", "grate", "up grate"}),
                "short" : "a grate leading up",
                "long" : "This is the down side of a grate leading up. It looks very heavy.",
                "lockable" : 0 ]) );

    SetPerforated(1);
    SetClosed(1);
    SetLocked(0);
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

void init(){
    ::init();
}
