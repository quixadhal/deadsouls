#include <lib.h>
#include <rooms.h>

inherit LIB_ROOM;

void create() {
    room::create();
    SetAmbientLight(30);
    SetShort("the furnace");
    SetLong("The furnace. Things that arrive here are "+
      "incinerated. You probably shouldn't be here. Go down to get out.");
    SetProperties(([
	"no attack" : 1,
      ]));
    SetExits( ([ "down" : ROOM_START ]) );
    call_out((: reload_room :), 600, load_object(base_name(this_object())));
    set_heart_beat(1);
    SetNoModify(1);
}
int CanReceive(object ob){
    if(ob && interactive(ob)){
	tell_object(ob, "You fail to enter the furnace.");
	return 0;
    }
    return 1;
}
void init(){
    ::init();
}
void heart_beat(){
    if(sizeof(all_inventory(this_object()))){
	foreach(object ob in deep_inventory(this_object())){
	    ob->eventDestruct();
	}
    }
}
