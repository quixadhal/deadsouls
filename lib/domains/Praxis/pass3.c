#include <lib.h>

inherit LIB_ROOM;

void create() {
    ::create();
    SetAmbientLight(25);
    SetShort( "a mountain pass");
    SetLong(
            "You are west of the highest point of the Daroq Mountains. "
            "Off to the south you can see some less imposing "
            "mountains, known as the Destiny Mountains. The trail "
            "which you are traveling rises high above the rest of "
            "the mountain range, allowing you a spectacular view "
            "of the lands of Nightmare. The path leads east and west." );
    SetItems(
            (["point" : "From here it looks like you can see the entire world.",
             "pass" : "The pass heads down into the loneliness of the "
             "Qajip Desert.",
             "mountains" : "A huge mountain range separating the fertile "
             "lands of the east from the desert.",
             "valley" : "A huge forested valley.",
             "desert" : "The Qajip Desert where many travellers have "
             "lost their way and not been found.",
             "mountain" : "It is one of the lower ones, guarding the pass."]) );
    SetSkyDomain("town");
    SetExits( ([ 
                "east" : "/domains/Praxis/pass2",
                ]) );
}
void reset() {
    ::reset();
    if(!present("balrog")) 
        new("/domains/Praxis/obj/mon/balrog")->move(this_object());
}
void init(){
    ::init();
}
