#include <lib.h>
#include <rooms.h>

inherit LIB_ROOM;

void create() {
    room::create();
    SetAmbientLight(30);
    SetShort("the furnace");
    SetLong("The furnace. Things that arrive here are periodically "+
      "incinerated. You probably shouldn't be here. Go down to get out.");
    SetProperties(([
	"no attack" : 1,
      ]));
    SetExits( ([ "down" : ROOM_START ]) );
    call_out((: reload_room :), 360, load_object(base_name(this_object())));
    SetNoModify(1);
}
int CanReceive(object ob){
    return 1;
}
void init(){
    ::init();
}
