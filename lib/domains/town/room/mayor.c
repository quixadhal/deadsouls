#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetTown("town");
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("Mayor's office");
    SetLong("This is the rather cramped office of the town's mayor. Despite\na fair amount of clutter, it seems well suited to the mayor's \nworking style and habits.");
    SetProperties (([
                "no attack":1, 
                "no bump":1, 
                "no steal":0, 
                "no magic":0, 
                "no paralyze":0, 
                "no teleport":0]));
    SetItems(([  
                "clutter" : "There are papers and folders everywhere, but the stacks and piles seem somehow a well-organized mess." ,
                ({"mess","stack","stacks","pile","piles","papers","paper","folder","folders"}) : "Evidence of a busy man and strangely-organized mind."]));
    SetExits( ([ 
                "down" : "/domains/town/room/thall",
                ]) );
    SetInventory(([  "/domains/town/npc/mayor" : ({60, 1})]));
}
void init() {
    ::init();
    SetSmell(([]));
    SetListen(([]));
}
