/*    /domains/Ylsrim/room/armory_storage.c
 *    from the Dead Souls Mud Library
 *    created by Descartes of Borg 960302
 */

#include <lib.h>

inherit LIB_ROOM;

static void create() {
    room::create();
    SetNoClean(1);
    SetTown("Ylsrim");
    SetProperties( ([ "login" : "/domains/Ylsrim/room/"+ "armory" ]) );
    SetClimate("indoors");
    SetShort("armory storage");
    SetLong("Armory storage.");
    SetObviousExits("west");
    SetExits( ([ "west" : "/domains/Ylsrim/room/"+ "armory" ]) );
}

int CanReceive(object ob) {
    if( !living(ob) || creatorp(ob) ) return room::CanReceive(ob);
    else return 0;
}
void init(){
    ::init();
}
