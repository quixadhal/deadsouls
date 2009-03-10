/*    /domains/Ylsrim/room/sand_hole.c
 *    From the Dead Souls Mud Library
 *    An example of a room in which you go after digging (get trapped!)
 *    Created by Descartes of Borg 961231
 */

#include <lib.h>

inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(25);
    SetShort("a dark hole in the sand");
    SetLong("You are in a very dark, sandy hole in the desert.");
    AddItem("sand", "It is everywhere.");
    SetObviousExits("");
    SetExits( ([ "up" : "/domains/Ylsrim/room/"+ "sand_room" ]));
    // use a door to act as a hole, as that is what the hole really is
    SetDoor("up", "/domains/Ylsrim/etc/dug_hole");
}
void init(){
    ::init();
}
