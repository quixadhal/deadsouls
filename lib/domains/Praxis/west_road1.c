#include <lib.h>

inherit LIB_ROOM;

void create() {
    ::create();
    SetProperty("light", 2);
    SetProperty("night light", 1);
    SetProperty("no castle", 1);
    SetShort( "the southern end of West Road");
    SetLong( 
      "West Road is the main residential road that runs through Praxis. "
      "One or two more houses lie on either side of the road, until "
      "it comes to a dead end south of here at Boc La Road. The small "
      "huts lining the road are rather run down and unattractive. You "
      "can hear noises coming from inside of them, as the inhabitants "
      "carry on with their lives.");
    SetItems(
      (["road" : "West Road, which marks the westernmost end of Praxis.",
        "huts" : "They are small peasant huts.",
        "hut" : "It has a thatch roofing, and is not really extravagant."
      ]) );
    SetSkyDomain("town");
    SetExits( 
      (["north" : "/domains/Praxis/west_road2",
        "south" : "/domains/Praxis/w_boc_la3"]) );
}
void init(){
    ::init();
}
