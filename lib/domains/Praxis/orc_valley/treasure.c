#include <lib.h>

inherit LIB_ROOM;

void create() {
    ::create();
    SetProperty("light", 1);
    SetProperty("indoors", 1);
    SetProperty("no castle", 1);
    SetSmell("default", "It smells very musty in here.");
    SetShort( "Treasury of the Valley Orcs");
    SetLong(
            "This small vault where the orc of the valley keep their "
            "treasure is poorly lit and littered with dust, making you well "
            "aware that the orcs willingly allow anyone in here.");
    SetItems(
            (["vault" : "It is very, very dimly lit."]) );
    SetExits( (["south" : "/domains/Praxis/orc_valley/shaman"]) );
    SetNoClean(1);
    SetProperty("no teleport", 1);
}

void reset() {
    ::reset();
    if(!present("chest")) 
        new("/domains/Praxis/obj/misc/chest")->move(this_object());
    //present("chest", this_object())->SetClosed(0);
    //if(!present("orcslayer", present("chest")))
    new("/domains/Praxis/obj/weapon/orc_slayer")->move(present("chest"));
    //present("chest")->SetClosed(1);
}
void init(){
    ::init();
}

