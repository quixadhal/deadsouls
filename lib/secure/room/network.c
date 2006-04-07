#include <lib.h>
#include <daemons.h>
inherit LIB_ROOM;

static void create() {
    object ob;
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("Network room");
    SetLong("This is where all network messages go. It is an extremely "
      "noisy and uncomfortable location, just like network rooms all around "
      "the world. It is here for debugging and troubleshooting purposes, so "
      "if that's not what you're doing, you should probably leave." );
    SetExits(([
	"up" : "/secure/room/arch",
      ]));

}
int CanReceive(object ob) {
    if( living(ob) && !archp(ob)  ){
	message("info","The network room is available only to "+
	  "admins, sorry.",ob);
	return 0;
    }
    return 1;
}
void init(){
    ::init();
}
