/*    /domains/Ylsrim/room/church
 *    From the Dead Souls Mud Library
 *    An example of a religious class hall
 *    Created by Descartes of Borg 961222
 */

#include <lib.h>

inherit LIB_ROOM;

int CheckPriest(string dir) {
    object ob;

    if( this_player()->ClassMember("cleric") || creatorp(this_player()) ) {
        return 1;
    }
    if( ob = present("cleric", this_object()) ) {
        ob->eventForce("speak I cannot allow you in this sacred rooms.");
        return 0;
    }
    else {
        return 1;
    }
}

static void create() {
    room::create();
    SetNoClean(1);
    SetTown("Ylsrim");
    SetClimate("indoors");
    SetAmbientLight(27);
    SetShort("Ylsrim Church");
    SetLong("Dim candles create more than enough light to fill this "
            "very humble church of the Dalin religion.  Dalites come "
            "here to pray, and others come to convert to Dalin.");
    AddItem("candle", "They do not appear to make much light, but the "
            "church is very bright.", ({ "dim" }));
    AddItem(new("/domains/Ylsrim/etc/church_button"));
    SetExits(([ 
                "south" : "/domains/Ylsrim/room/"+ "kaliid3"]));
    SetDoor("south", "/domains/Ylsrim/etc/church_door");
    SetInventory(([ "/domains/Ylsrim/npc/priest" : 1 ]));
}
void init(){
    ::init();
}
