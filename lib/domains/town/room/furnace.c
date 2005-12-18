#include <lib.h>
#include <rooms.h>

inherit LIB_ROOM;

void create() {
    room::create();
    SetShort("the furnace");
    SetLong("The furnace. Things that arrive here are periodically "+
      "incinerated. You probably shouldn't be here. Go down to get out.");
    SetExits( ([ "down" : ROOM_START ]) );
    call_out((: reload_room :), 360, load_object(base_name(this_object())));
    SetNoModify(1);
}

int CanReceive(object ob){
    //    if(!interactive(ob)){
    //	ob->eventDestruct();
    //	return 1;
    //   }
    //  ob->eventMove(ROOM_VOID);
    return 1;
}
