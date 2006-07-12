/* 
File created with Roommaker 
written by Haderach@Frontiers 
Room made by Cratylus
Date: Thu Oct 20 21:08:46 2005
WARNING! WARNING! If you edit this room by hand the room maker may be unable to read
it properly. Hand-edit at your own risk.

*/

#include <lib.h>
#include "../customdefs.h";
inherit LIB_ROOM;

static void create() {
    room::create();
    SetTown("fort");
    SetClimate("indoors");
    SetDayLight(30);
    SetNightLight(15);
    SetShort("stables");
    SetDayLong("These are the fortress stables. It appears to be a fairly professional operation, with fresh hay provided and facilities for feeding and grooming large animals. Various bays are designed to accomodate the beasts, and a common trough provides water. Warehouse road is east.");
    SetNightLong("These are the fortress stables. It appears to be a fairly professional operation, with fresh hay provided and facilities for feeding and grooming large animals. Various bays are designed to accomodate the beasts, and a common trough provides water. Warehouse road is east.");
    SetProperties (([
	"no attack":0, 
	"no bump":0, 
	"no steal":0, 
	"no magic":0, 
	"no paralyze":0, 
	"no teleport":0]));
    SetItems(([      "hay" : "This stuff is a kind of semi-dried straw grass which provides feed for the animals, bedding for mounts which can lie down, and something for people to walk on that isn't dirt or feces." ,
	({"stable", "stables", "bay", "bays", "here", "facilities", "operation"}) : "Typical of small-to-medium strongholds, this professionally-kept stable is about as neat and clean as one could ever hope of a building that houses animals." ,
	({"manure", "feces", "crap", "shit", "poop", "excrement"}) : "This is the waste product of animals. It is evident that the stuff is cleaned up as much as possible, but animals being animals, there's only so much one can do." ,
	"trough" : "A long trough that can be filled with water to provide hydration to livestock."]));
    SetInventory(([  MY_NPC + "/" +"horse" : 1]));
    SetObviousExits("e");
    AddExit("east", MY_ROOM + "/" +"sroad");
    AddExit("out", MY_ROOM + "/" +"sroad");
    //funs
    //snuf
}
void init() {
    room::init();
    SetSmell(([  "default" : "The strong smell of manure is nearly overpowering."]));
    SetListen(([  "default" : "The occasional snort and whinny of animals can be heard."]));
}
//extras
