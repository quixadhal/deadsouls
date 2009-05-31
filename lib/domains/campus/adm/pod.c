/*    /domains/campus/adm/void.c
 *    from the Dead Souls Object Library
 *    place where people go when their environments accidentally are
 *    destructed
 *    created by Descartes of Borg 960302
 */

#include <lib.h>
#include ROOMS_H
#include <daemons.h>

inherit LIB_ROOM;

void create() {
    room::create();
    SetShort("the incept pod");
    SetLong("The incept pod. Some objects come here to be created "+
            "and identified. Go down to get out.");
    SetExits( ([ "down" : ROOM_START ]) );
}
int CanReceive(object ob){
    if(!interactive(ob)){
        ob->eventDestruct();
        return 1;
    }
    ob->eventMove(ROOMS_D->GetVoid(ob));
    return 1;
}
