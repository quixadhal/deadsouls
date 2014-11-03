#include <lib.h>

inherit LIB_ROOM;

void create() {
    ::create();
    SetProperty("no castle", 1);
    SetProperty("light", 2);
    SetProperty("night light", 1);
    SetSmell("default", "An unbelievable stench fills the air.");
    SetSmell("dump", "It smells of all kinds of "
            "nasty rotting things.");
    SetShort( "The end of a nasty alley");
    SetLong(
            "This is the end of the alley. All about you can see "
            "decaying buildings. Off to the south you can see an old abondoned
            building, worn from the weather and a lack of care. To the north "
            "there are more ruins. It appears that this used to be a whole "
            "other section of Praxis, abondened long ago. The Praxis dump "
            "is further west." );
    SetItems(
            (["alley" : "It goes through the worst parts of Praxis.",
             "dump" : "People take their trash there. Always "
             "during daytime hours.",
             "building" : "It looks like no one has paid it any "
             "attention in eons.",
             "ruins" : "Graffiti covers them."]) );
    SetSkyDomain("town");
    SetExits( 
            ([ "east": "/domains/Praxis/alley1", 
             "north":"/domains/Praxis/wall",
             "south":"/domains/Praxis/building", 
             "west":"/domains/Praxis/dump" ]) );
}
void init(){
    ::init();
}
