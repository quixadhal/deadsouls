/*    /domains/Ylsrim/room/sand_room.c
 *    From the Dead Souls V Object Library
 *    An example of a room in which you can dig
 *    Created by Descartes of Borg 961231
 */

#include <lib.h>

inherit LIB_ROOM;
inherit LIB_DIG;  // inherit this to add dig functionality

// This function gets called when someone tries to bury the hole
mixed buryHole(object who, object shovel) {
    int closed = "/domains/Ylsrim/etc/dug_hole"->GetClosed();

    if( closed ) {
	who->eventPrint("Perhaps you want to dig the hole?");
	return 1;
    }
    send_messages("bury", "$agent_name $agent_verb a deep, gaping "
		  "hole in the sand.", who, 0, environment(who));
    "/domains/Ylsrim/etc/dug_hole"->SetClosed(1);
    return 1;
}

// This function gets called when someone tries to dig
mixed digHole(object who, object shovel) {
    int closed = "/domains/Ylsrim/etc/dug_hole"->GetClosed();

    if( !closed ) {
	who->eventPrint("Perhaps you want to bury the hole?");
	return 1;
    }
    send_messages("dig", "$agent_name $agent_verb up a deep, gaping "
		  "hole in the sand.", who, 0, environment(who));
    "/domains/Ylsrim/etc/dug_hole"->SetClosed(0);
    return 1;
}

// This function is set in SetLong() to describe the room differently
// when the hole is open.
string describeRoom() {
    string str = "You are just outside the western edge of Ylsrim.  You now "
	"stand in the open desert with a dry, arid wind draining you "
	"of all strength.  Nothing at all exists here besides endless "
	"sand.  About all you could possibly do is sit and dig.";

    if( "/domains/Ylsrim/etc/dug_hole"->GetClosed() ) {
	return str;  // no hole to see
    }
    else {
	return str + "  A hole opens up in the middle of the sand.";
    }
}
    
static void create() {
    room::create();
    SetClimate("arid");
    SetShort("a sandy desert outside of Ylsrim");
    SetLong((: describeRoom :));
    AddItem("sand", "Boring stretches of sand extend to the horizon.");
    AddItem("ylsrim", "It is east of here.");
    SetObviousExits("e");
    SetExits( ([ "east" : "/domains/Ylsrim/room/"+ "kaliid7",
	       "down" : "/domains/Ylsrim/room/"+ "sand_hole" ]));
    // use a door to act as a hole, as that is what the hole really is
    SetDoor("down", "/domains/Ylsrim/etc/dug_hole");
    // this tells the room to call this function when a player digs
    SetDig((: digHole :));
    // this tells the room what to call when a player buries the hole
    SetBury((: buryHole :));
}
void init(){
::init();
}
