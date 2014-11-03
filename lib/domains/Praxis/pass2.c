#include <lib.h>

inherit LIB_ROOM;

void create() {
    ::create();
    SetAmbientLight(25);
    SetShort( "a mountain pass");
    SetLong(
            "You are at the zenith of the dark pass which runs through "
            "the Daroq Mountains. Looking down below, you can see "
            "the barren Qajip Desert off to the west. It is rumored "
            "that upon entering the desert, few make it out alive. "
            "Far off to the east you can see the gentle swells of "
            "a forest valley. Directly ahead a small cave opens "
            "up into the mountain.");
    SetItems(
            (["point" : "From here it looks like you can see the entire world.",
             "pass" : "It winds around from the west to the southeast.",
             "mountains" : "A huge mountain range separating the fertile "
             "lands of the east from the desert.",
             "valley" : "A huge forested valley.",
             "desert" : "The Qajip Desert where many travellers have "
             "lost their way and not been found.",
             "forest" : "It is dark and gloomy near the mountains, but "
             "it looks friendlier off to the east.",
             "cave" : "The cave stands very darkly and ominously before you.",
             "mountain" : "It is one of the lower ones, guarding the pass."]) );
    SetSkyDomain("town");
    SetExits( 
            (["west" : "/domains/Praxis/pass3",
             "southeast" : "/domains/Praxis/pass1",
             "north"	 : "/domains/Praxis/mountains/entrance"]) );
}
void init(){
    ::init();
}
