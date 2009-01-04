#include <lib.h>

inherit LIB_ROOM;

void create() {
    ::create();
    SetShort( "The Praxis sewers");
    SetLong(
            "You are underneath the town of Praxis inside their "
            "small sewer system.  There is a manhole to the surface above "
            "you.");
    SetSmell("default", "The sewers stink of humanoid waste.");
    SetDoor("/domains/Praxis/yard","up");
    SetProperty("light", 0);
    SetProperty("indoors", 1);
    SetItems(
            ([ "manhole" : "It is a hole that leads to the surface."]) );
    SetExits( ([ 
                "up" : "/domains/Praxis/yard",
                "east" : "/domains/Praxis/sewer",
                ]) );
}

void reset() {
    ::reset();
    if(!present("sword")) 
        new("/domains/Praxis/obj/weapon/sword")->move(this_object());
}
void init(){
    ::init();
}
