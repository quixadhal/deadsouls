/*    /domains/Ylsrim/virtual/desert.c
 *    from the Dead Souls V Object Library
 *    created by Descartes of Borg 960302
 */

#include <lib.h>
#define __DIR__ "/domains/Ylsrim/virtual/"

inherit LIB_ROOM;

static private int XPosition, YPosition;

static void SetLongAndItems();

varargs static void create(int x, int y) {
    string n, s, e, w;

    SetNoReplace(1);
    room::create();
    XPosition = x;
    YPosition = y;
    SetClimate("arid");
    SetProperties( ([ "light" : (x == 25 ? 2 : 3 ) ]) );
    SetShort(x == 25 ? "the edge of a desert" : "deep within a desert");
    SetLongAndItems();
    if( x == 25 ) e = "desert/" + random(25) + "," + y;
    else e = "desert/" + (x+1) + "," + y;
    if( x == 1 ) w = "desert/" + random(25) + "," + y;
    else w = "desert/" + (x-1) + "," + y;
    if( y == 25 ) n = "desert/" + x + "," + random(25);
    else n = "desert/" + x + "," + (y+1);
    if( y == 1 ) s = "desert/" + x+ "," + random(25);
    else s = "desert/" + x + "," + (y-1);
    SetGoMessage("The dunes are too high to let you pass in that direction.");
    if( n ) AddExit("north", __DIR__ + n);
    if( s ) AddExit("south", __DIR__ + s);
    if( e ) AddExit("east", __DIR__ + e);
    if( w ) AddExit("west", __DIR__ + w);
}
    
static void SetLongAndItems() {
    mapping inv, items;
    string str;

    inv = ([]);
    str = "You are at the heart of a vast desert.  The emptiness of endless "
      "sand cuts at your morale with the strength of a scimitar.  No path "
      "marks your way.";
    items = ([ "desert" : "It is so vast." ]);
    if( !random(50) ) {
	str += "  Burnt wood, scattered rocks and twigs, and other signs "
	  "of an abandoned camp site are scattered about.";
	items[({ "twigs", "sticks", "kindling", "wood", "burnt wood" })] = 
	      "Though long since burnt to nothing, scattered kindling "
	      "and burnt wood lie about as a memory of travellers who have "
	      "passed through";
	if( random(2) ) {
	    string thing;

	    foreach(thing in ({ "twigs", "sticks", "kindling", "wood" })) 
	      SetSearch(thing, function(object who, string str) {
		  object ob;
		  string thing2;

		  if( !(ob = new("/domains/Ylsrim"+ "/etc/pole")) )
		    return 0;
		  who->eventPrint("You find a fishing pole!");
		  eventPrint((string)who->GetName() + " finds a fishing pole "
			     "among the abandoned campsite.", who);
		  foreach(thing2 in ({ "twigs", "sticks", "kindling", "wood"}))
		    RemoveSearch(thing2);
		  if( !((int)ob->eventMove(this_player())) ) {
		      who->eventPrint("You drop the pole!");
		      eventPrint((string)who->GetName() + " drops the pole.",
				 who);
		      ob->eventMove(this_object());
		  }
		  return 1;
	      });
	}
    }
    else if( !random(10) ) 
      SetSmell("default", "You smell a distant camp fire.");
    if( !random(25) )
      inv["/domains/Ylsrim"+ "/npc/traveller"] = random(3) + 1;
    else if( !random(4) ) 
      SetListen("default", "You hear voices whispering in the distance.");
    SetLong(str);
    SetItems(items);
    SetInventory(inv);
}
