#include <lib.h>

inherit LIB_ROOM;

void create() {
    ::create();
    SetProperty("light", 3);
    SetProperty("no castle", 1);
    SetShort( "a dirt highway");
    SetLong(
      "You find yourself on a large dirt highway which heads "
      "west out of Praxis. In the distance the Daroq Mountains " 
      "rise majestically over the flat plains of the rest of the lands. "
      "This area is rather sparse, with just a small smattering of trees. "
      "There appears to have been grass here a long time ago, but it has "
      "been ruined from the many adventurers that traverse these parts. "
      "East leads to West Boc La road and east takes you to the highway ");
    SetItems(
      (["highway" : "A highway linking Praxis to the western lands "
        "of Nightmare.",
        ({ "trees", "tree" }) : "The trees are very small. This isn't "
        "a very good environment for them to grow in.",
        "village" : "A tiny adventuring village. They have many "
        "services there.",
        "mountains" : "The Daroq Mountains are far to the west."
      ]) );
    SetSkyDomain("town");
    SetExits( 
      (["west" : "/domains/Praxis/highway2",
        "east" : "/domains/Praxis/w_boc_la3"]) );
}
void init(){
    ::init();
}
