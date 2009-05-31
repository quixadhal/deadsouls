/*    /realms/trent/commons/fountain.c
 *     "the Fountain"
 *     Trent@LPC University
 */

#include <lib.h>

inherit LIB_ROOM;
inherit LIB_AMBIANCE;

int CheckDoor(){
    object *obs;
    int i;
    obs=get_livings(find_object("/domains/campus/room/sewer2"),1);
    if(!sizeof(obs) && !"/domains/campus/doors/grate"->GetClosed() ) {
        "/domains/campus/doors/grate"->SetClosed(1);
        tell_object(this_object(),"The drainage grate slams shut.");
    }
    return 1;
}

static void create() {
    string day,night;
    room::create();
    //SetProperty("light", 2);
    SetAmbientLight(30);
    SetClimate("temperate");
    SetShort("Fountain square");
    day="This is Fountain Square, the center of Larstown.  "+
        "A small, beautiful fountain is the center of this cobblestone "+
        "square. A circular stone bench surrounds the edge of the fountain's pool, "+
        "and water that spills over from the fountain drains into a rainwater grate "+
        "set into the ground.\n "+
        "\tLarstown is still a small, modest village, but renovations such as this "+
        "tasteful commons and the fountain make it clear it remains a vibrant, "+
        "active place to live in. You may leave this square by North Street, East Avenue, "+
        "South Road, or West Track. "; 
    night="Normally quite busy in the daytime, the square is now filled only " +
        "with the sounds of the fountain's flow.";	
    SetDayLong(day);
    SetNightLong(day+night);
    SetItems( ([ ({ "square", "center" }) : "The square is a popular meeting place "
                "for Larstown's denizens.",
                "fountain" : "This beautifully designed fountain was donated "
                "by Trent.",
                ({"street","North Street","north street"}) : "North Street leads north "+
                "toward the village marketplace and its clock tower.",
                ({"avenue","east avenue","East Avenue"}) : "East Avenue leads east toward "+
                "the shore and Larstown docks.",
                ({"road","south road","South Road"}) : "South Road leads south toward "+
                "the town's water well, bank, and residential area.",
                ({"track","West Track","west track"}) : "West Track leads west toward the "+
                "town hall and some business and civic buildings.",
                ({"circular bench", "bench", "stone bench", "stone"}) : "Many of "
                "Frontier's denizens like to sit around the fountain and enjoy the "
                "beautiful sights and sounds of the square." ]) );
    SetSearch( ([ "fountain" : "It appears someone has already stolen all the coins in the fountain." ]) );
    SetSmell( ([ "default" : "You smell the clean and misty air of the fountain's spray." ]) );
    SetObviousExits("n,s,e,w");
    SetExits( ([
                "east" : "/domains/campus/room/square",
                "east" : "/domains/campus/room/square",
                "east" : "/domains/campus/room/square",
                "east" : "/domains/campus/room/square",
                "down" : "/domains/campus/room/sewer2"
                ]) );

    SetDoor("down","/domains/campus/doors/grate");

}

void init(){
    CheckDoor();
}
