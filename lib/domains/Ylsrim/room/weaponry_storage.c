/*    /domains/Ylsrim/room/weaponry_storage.c
 *    from the Dead Souls V Object Library
 *    created by Descartes of Borg 960302
 */

#include <lib.h>

inherit LIB_ROOM;

static void create() {
    room::create();
    SetNoClean(1);
    SetTown("Ylsrim");
    SetProperties( ([ "login" : "/domains/Ylsrim/room/"+ "weaponry" ]) );
    SetClimate("indoors");
    SetShort("weaponry storage");
    SetLong("Weaponry storage.");
    SetObviousExits("e");
    SetExits( ([ "west" : "/domains/Ylsrim/room/"+ "weaponry" ]) );
}

int CanReceive(object ob) {
    if( !living(ob) || creatorp(ob) ) return room::CanReceive(ob);
    else return 0;
}
void init(){
::init();
}
