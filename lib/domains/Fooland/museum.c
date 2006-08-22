#include <mudlib.h>

inherit ROOM ;

void create() {
	::create();
	set ("light", 1) ;
	set ("long",
"This is a tiny museum in the village of Smallville. Here can be found the\n" +
"fascinating history of the MUD. Items around the room include a sword, a\n"+
	"map, and a helmet.\n") ;
	set ("short", "Museum in Smallville.") ;
	set ("exits", ([ "north" : "/domains/grid/rooms/11,3.grid" ]) ) ;
	set ("item_desc", ([ "sword" :
	"This is the sword of the mighty Sir Foo, founder of Footown. With it, he\n"+
	"slew the dragon Bazz, who lived in a cave in the Bazz mountains, and made\n" +
	"the realm safe for mortal men.\n",
	"map" : "This is a map of the Bazz Mountains, showing the location of the cave where\n" +
	"the mighty dragon Bazz once lived. It shows Bazz's cave being located just\n" +
	"to the west of a small valley in the mountains.\n",
	"helmet" : "This is the helmet of Gazonga, Sir Foo's noble servant-at-arms. Gazonga was\n" +
	"killed in the battle between Foo and the mighty dragon Bazz.\n" ]) ) ;
}
