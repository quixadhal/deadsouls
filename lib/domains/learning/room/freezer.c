#include <lib.h>
#include ROOMS_H

inherit LIB_ROOM;

void create() {
    room::create();
    SetNoClean(1);
    SetProperties(([ "login" : ROOM_START ]));
    SetShort( "The freezer");
    SetLong( "The local freezer.  Go down to leave.");
    SetObviousExits("down");
    SetExits( ([ "down" : ROOM_START ]) );
    call_out("clean_room", MAX_NET_DEAD_TIME);
    SetNoModify(1);
}

static void clean_room(){
    object ob;
    call_out((: clean_room :), MAX_NET_DEAD_TIME);
    foreach(ob in filter(all_inventory(), (: living($1) :))){
        string name = last_string_element(base_name(ob),"/");
        if(!user_exists(name)) continue;
        ob->eventDestruct();
    }
}

void init(){
    ::init();
}
