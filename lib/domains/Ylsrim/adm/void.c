/*    /domains/Ylsrim/adm/void.c
 *    from the Dead Souls V Object Library
 *    place where people go when their environments accidentally are
 *    destructed
 *    created by Descartes of Borg 960302
 */

#include <lib.h>
#include <rooms.h>

inherit LIB_ROOM;

void create() {
    room::create();
    SetShort("the void");
    SetLong("The void.  Go down to get out.");
    SetExits( ([ "down" : ROOM_START ]) );
}
