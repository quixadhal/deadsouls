/*    /domains/Ylsrim/room/armoury.c
 *    from the Dead Souls V Object Library
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
    SetAmbientLight(27);
    SetShort("the Ylsrim armoury");
    SetLong((: CheckOpen :));
    SetItems( ([ ({ "case", "cases" }) : (: CheckItem :),
      "bazaar" : "People from all about are wandering around, looking "
      "for a bargain." ]) );
    SetObviousExits("w");
    SetExits( ([ "west" : "/domains/Ylsrim/room/"+ "bazaar" ]) );
    SetInventory( ([ "/domains/Ylsrim"+ "/npc/max" : 1 ]) );
}

string CheckOpen(string str) {
    object ob;

    ob = present("vendor");
    if( query_night() ) {
	if( ob ) ob->eventDestruct();
	return ("The cases of the armoury are empty, as the armoury "
		"has closed for the night.  The bazaar is "
		"west.");
    }
    if( !ob )
      return ("Fresh blood is splattered across broken cases which once "
	      "housed display armour sold in this armoury.  The armour is "
	      "gone, and the vendor clearly murdered.  All you can do is "
	      "shake your head and exit to the west.");
    return ("Sealed cases contain display armour, exemplifying what the local "
	    "vendor, Max, has to offer.  He buys and sells all sorts of "
	    "armour from adventurers who come here to sell of their "
	    "discovered goods, or equip to continue adventuring.  An exit "
	    "is west.");
}

string CheckItem(string str) {
    if( str == "case" || str == "cases" ) {
	if( query_night() ) return "They are empty.";
	else if ( present("vendor") ) return "They are filled with armour.";
	else return "They are broken and covered in blood.";
    }
}
	
void init(){
::init();
}
