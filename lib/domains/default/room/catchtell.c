#include <lib.h>
#include <daemons.h>
inherit LIB_ROOM;

static void create() {
    object ob;
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("catch_tell room");
    SetLong("This is a room designed to receive certain messages from NPC's. It is largely for debugging purposes, and it is not vital to normal mud operations.");
    SetExits(([
                "west" : "/secure/room/network",
                ]));

}
int CanReceive(object ob) {
    if( !archp(ob) ){
        message("info","The catch tell room is available only to "+
                "admins, sorry.",ob);
        return 0;
    }
    return 1;
}
void init(){
    ::init();
}
