/*    /domains/Ylsrim/room/armoury_storage.c
 *    from the Dead Souls V Object Library
 *    created by Descartes of Borg 960302
 */

#include <lib.h>

inherit LIB_ROOM;

static void create() {
    room::create();
    SetNoClean(1);
    SetTown("Ylsrim");
    SetProperties( ([ "login" : __DIR__ "armoury" ]) );
    SetClimate("indoors");
    SetShort("armoury storage");
    SetLong("Armoury storage.");
    SetObviousExits("w");
    SetExits( ([ "west" : __DIR__ "armoury" ]) );
}

int CanReceive(object ob) {
    if( !living(ob) || creatorp(ob) ) return room::CanReceive(ob);
    else return 0;
}
