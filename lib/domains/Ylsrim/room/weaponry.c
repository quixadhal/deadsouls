/*    /domains/Ylsrim/room/weaponry.c
 *    from the Dead Souls Mud Library
 *    created by Descartes of Borg 951023
 */

#include <lib.h>
#include <dirs.h>

inherit LIB_ROOM;

string CheckOpen(string str);
string CheckItem(string str);

static void create() {
    room::create();
    SetTown("Ylsrim");
    SetClimate("indoors");
    SetAmbientLight(28);
    SetShort("the Ylsrim weaponry");
    SetLong((: CheckOpen :));
    SetItems( ([ ({ "case", "cases" }) : (: CheckItem :),
                "bazaar" : "People from all about are wandering around, looking "
                "for a bargain." ]) );
    SetObviousExits("east");
    SetExits( ([ "east" : "/domains/Ylsrim/room/"+ "bazaar" ]) );
    SetInventory( ([ "/domains/Ylsrim"+ "/npc/shiela" : 1 ]) );
}

string CheckOpen(string str) {
    object ob;

    ob = present("vendor");
    if( query_night() ) {
        if( ob ) ob->eventDestruct();
        return ("The cases of the weaponry are empty, as the weaponry "
                "has closed for the night.  The bazaar is "
                "east.");
    }
    if( !ob )
        return ("Fresh blood is splattered across broken cases which once "
                "housed display weapons sold in this weaponry.  The weapons are "
                "gone, and the vendor clearly murdered.  All you can do is "
                "shake your head and exit to the east.");
    return ("Sealed cases contain display weapons, exemplifying what the "
            "local vendor has to offer.  She buys and sells all sorts of "
            "weapons from adventurers who come here to sell of their "
            "discovered goods, or equip to continue adventuring.  An exit "
            "is east.");
}

string CheckItem(object ob) {
    if( query_night() ) return "They are empty.";
    else if ( present("vendor") ) return "They are filled with weapons.";
    else return "They are broken and covered in blood.";
}

void init(){
    ::init();
}
