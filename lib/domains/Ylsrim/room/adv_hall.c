/*    /domains/Ylsrim/room/adv_hall.c
 *    from the Dead Souls Mud Library
 *    created by Descartes of Borg 960302
 */

#include <lib.h>

inherit LIB_ROOM;

static void create() {
    object ob;

    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("the adventurer's hall");
    SetLong("A bulletin board sits in the middle of this circular room where "
            "adventurers from all over gather to meet one another.  Kaliid Road "
            "lies to the south.  Stairs lead up to the tower.");
    SetItems( ([ "stairs" : "They spiral up into the tower which looks over "
                "Kaliid Road.",
                "tower" : "You can't see much in the darkness into which the "
                "stairs disappear." ]));
    SetExits( ([
                "up" : "/domains/Ylsrim/room/"+ "tower",
                "south" : "/domains/Ylsrim/room/"+ "kaliid4" ]) );
    ob = new("/domains/Ylsrim/etc/adv_board");
    ob->set_max_posts(30);
    ob->eventMove(this_object());
}
void init(){
    ::init();
}
