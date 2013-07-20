#include <lib.h>

inherit LIB_ROOM;

void create() {
    ::create();
    SetProperties( (["light":3, "no castle" : 1 ]) );
    SetShort( "eastern end of the Daroq Mountains pass");
    SetLong( "Looking down this great ravine you can see the "
      "Great Western Highway winding its way east across "
      "the land. Veering off to the left, this path leads "
      "into the Daroq Mountains, where it is rumored that "
      "goblins, balrogs and tak'daroqs live. The territory "
      "is very rough and dangerous, civilization is far "
      "behind you. Not too far off to the west is the highest "
      "point of the path.");
    SetItems( (["pass" : "A gloomy pass through the Daroq Mountains.",
        "highway" : "The Great Western highway.  It leads into Praxis.",
        "lands" : "The fertile lands of the east.",
        "range" : "The Daroq Mountains, home of many vile creatures.",
        "mountains" : "The Daroq Mountains, home of many vile creatures.",
        "territory" : "It belongs to the evil creatures of the underground.",
        "point" : "From there, the pass heads down westward into "
        "the Qajip Desert."]) );
    SetSkyDomain("town");
    SetExits( 
      (["northwest" : "/domains/Praxis/pass2",
        "east" : "/domains/Praxis/highway3"]) );
}
void init(){
    ::init();
}
