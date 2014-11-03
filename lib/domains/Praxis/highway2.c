#include <lib.h>

inherit LIB_ROOM;

void create() {
    ::create();
    SetProperties( (["light": 3, "no castle" : 1 ]));
    SetShort( "the great western highway");
    SetLong(
            "The Great Western Highway, extending both east and west, allows "
            "travel between the mountains and the many other villages of "
            "this land. The further west you travel, more dry and arid the air "
            "becomes. The highway has become very dusty, and starting to "
            "blend in with the rest of the earth, making the path almost "
            "indistinguishable. Looking beyond the Daraq mountains you "
            "can see the beginnings of the Qajip Desert.");
    SetItems(
            (["highway" : "A well-traveled dirt road between the "
             "mountains in the west and the populated towns of the east.",
             "lands" : "You are in a forested valley with the Daroq "
             "Mountains in the west.",
             "mountains" : "The Daroq Mountains, home of vile goblins.",
             "mountain" : "Trendahl Mountain.",
             "daroq mountains" : "Evil goblins live in the "
             "tunnels that thread the mountains."]) );
    SetSkyDomain("town");
    SetExits( ([ 
                "west" : "/domains/Praxis/highway3",
                "east" : "/domains/Praxis/highway1",
                ]) );
}

void reset() {
    ::reset();
    if(!present("bag")) 
        new("/domains/Praxis/obj/misc/bag")->move(this_object());
}
void init(){
    ::init();
}
