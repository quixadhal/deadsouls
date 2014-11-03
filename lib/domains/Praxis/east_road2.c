#include <lib.h>

inherit LIB_ROOM;

void create() {
    ::create();
    SetProperty("light", 2);
    SetProperty("night light", 1);
    SetProperty("no castle", 1);
    SetListen( "default", "The quiet chirp of grasshoppers hangs "
            "in the air." );
    SetShort( "the crossing of East Road and Sun Alley");
    SetLong(
            "Sun Alley, named for its' beautiful view of the sunsets, shoots "
            "off west from East Road. East Road continues, heading north and "
            "south upon the eastern border of town. To the east, you can see "
            "the Praxis cemetery, shaded by large trees. The air is very still "
            "here, and a feeling of foreboding hangs in the air.");
    SetItems(
            (["alley" : "A bright and cheery alley which marks the northern "
             "boarder of Praxis.",
             "road" : "East Road is a curvy road running north and south "
             "on the east side of Praxis.",
             "cemetery" : "The residents of Praxis get buried there. It "
             "looks spooky.",
             "trees" : "The trees are large drooping willow trees.",
             "forest" : "A great forest the marks the northern end of Praxis."])
            );
    SetSkyDomain("town");
    SetExits( 
            (["north" : "/domains/Praxis/east_road3",
             "south" : "/domains/Praxis/east_road1",
             "east" : "/domains/Praxis/cemetery/grave_yard",
             "west"	 : "/domains/Praxis/sun2"]) );
}

void init(){
    ::init();
}
