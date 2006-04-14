#include <lib.h>
#include <daemons.h>
inherit LIB_ROOM;

static void create() {
    object ob;
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("Router room");
    SetLong("This room is like the network room to its north, "
      "but it receives messages from the I3 router, instead of "
      "the intermud daemon. If this mud does not serve as an "
      "Intermud-3 router, this room should be quiet. Otherwise "
      "this may be the noisiest damn room on your mud.");
    SetExits(([
	"north" : "/secure/room/network",
      ]));

}
int CanReceive(object ob) {
    if( living(ob) && !archp(ob)  ){
	message("info","The router room is available only to "+
	  "admins, sorry.",ob);
	return 0;
    }
    return 1;
}
void init(){
    ::init();
}
